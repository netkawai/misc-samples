# Run specific command to obtain environement variable

import sys
import os
import glob
import fnmatch
import shlex
import re
import shutil
import subprocess
import platform

import enum

runSilently = False

class BashKind(enum.Enum):
    WSL = 1
    CYGWIN  = 2# MSYS also



isWindows = sys.platform.startswith("win")
isDarwin = sys.platform == "darwin"

def msg(text):
    if not runSilently:
        print(text)


def _runcmd(cmd, getOutput, echoCmd):
    """
    Runs a give command-line command, optionally returning the output.
    """
    if isinstance(cmd, list):
        # add quotes to elements of the command that need it
        cmd = cmd[:]
        for idx, item in enumerate(cmd):
            if " " in item or "\t" in item or ";" in item:
                if item[0] not in ['"', "'"]:
                    if '"' in item:
                        item = item.replace('"', '\\"')
                    item = '"{}"'.format(item)
                    cmd[idx] = item
        # convert the resulting command to a string
        cmd = " ".join(cmd)

    if echoCmd:
        msg(cmd)

    otherKwArgs = dict()
    if getOutput:
        otherKwArgs = dict(stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    sp = subprocess.Popen(cmd, shell=True, env=os.environ, **otherKwArgs)

    output = None
    if getOutput:
        outputEncoding = "cp1252" if sys.platform == "win32" else "utf-8"
        output = sp.stdout.read()
        output = output.decode(outputEncoding)
        output = output.rstrip()

    rval = sp.wait()

    return (rval, output)

def _which_in_shell(bash,cmd):
    """
        run which in bash.
    """
    if isWindows:
        cmd = '"{}" -l -c "which {}"'.format(bash, cmd)
    else:
        cmd = "which %s" % (cmd)

    return _runcmd(cmd,getOutput=False,echoCmd=False)

def _isBashKind(bash, kind):
    retval = 1

    if not os.path.exists(bash):
        return retval

    match kind:
        case BashKind.WSL:
            retval, out = _which_in_shell(bash,"wslpath")
        case _:
            # TODO: We need to distinguish MINGW vs MSYS
            retval, out = _which_in_shell(bash,"cygpath")
    return retval

def getBashPath(kind = None):
    """Check if there is a bash.exe on the PATH"""

    # first check under PATH
    bash = shutil.which("bash.exe")
    if bash:
        retval = _isBashKind(bash, kind)
        if retval == 0:
            return bash

    match kind:
        case BashKind.WSL:
            # if WSL should under PATH
            return bash # None
        case BashKind.CYGWIN:
            # system path for git bash
            bash = os.path.join(os.environ['ProgramFiles'],"Git","bin","bash.exe")
            retval = _isBashKind(bash,kind)
            if retval == 0:
                return bash
            # user path for git bash
            bash = os.path.join(os.environ['USERPROFILE'], "AppData","Local","Git","bash.exe")
            retval = _isBashKind(bash,kind)
            if retval == 0:
                return bash
            # default msys2 path      
            bash = os.path.join("C:\\msys2\\bin","bash.exe")
            retval = _isBashKind(bash,kind)
            if retval == 0:
                return bash
            # default cygwin path      
            bash = os.path.join("C:\\cygwin\\bin","bash.exe")
            retval = _isBashKind(bash,kind)
            if retval == 0:
                return bash
        case _:
            return defaultBash
    return bash

_cygwin = getBashPath(BashKind.CYGWIN)
_wsl = getBashPath(BashKind.WSL)

# Priority is CYGIN->WSL
defaultBash = _cygwin if _cygwin else _wsl




def runcmd(cmd, getOutput=True, echoCmd=True, fatal=True, onError=None):

    rval, output = _runcmd(cmd, getOutput, echoCmd)
    if rval:
        # Failed!
        #raise subprocess.CalledProcessError(rval, cmd)
        print("Command '%s' failed with exit code %d." % (cmd, rval))
        if getOutput:
            print(output)
        if onError is not None:
            onError()
        if fatal:
            sys.exit(rval)

    return output


def bash2dosPath(path):
    """
    Convert an absolute unix-style path to one Windows can understand.
    """

    # If we have cygwin then we can use cygpath to convert the path.
    # Note that MSYS2 (and Git Bash) now also have cygpath so this should
    # work there too.

    if defaultBash is _cygwin:
        path = runcmd('"{}" -l -c "cygpath -w {}"'.format(defaultBash,path),getOutput=True, echoCmd=False)
        return path
    elif defaultBash is _wsl:
        # Selected bash is WSL 
        wsl = shutil.which('wsl')
        path = runcmd('"{}" wslpath -a -w "{}"'.format(wsl,path), getOutput=True, echoCmd=False)
    else:
        raise RuntimeError('ERROR: Unable to find bash')

    return path

def dos2bashPath(path):
    """
    Convert an absolute dos-style path to one bash.exe can understand.
    """
    path = path.replace("\\", "/")

    # If we have cygwin then we can use cygpath to convert the path.
    # Note that MSYS2 (and Git Bash) now also have cygpath so this should
    # work there too.
    if defaultBash is _cygwin:
        path = runcmd('"{}" -l -c "cygpath -u {}"'.format(defaultBash,path),getOutput=True, echoCmd=False)
    elif wsl:
        # Selected bash is WSL 
        wsl = shutil.which('wsl')
        path = runcmd('"{}" wslpath -a -u "{}"'.format(wsl, path), getOutput=True, echoCmd=False)
        return path
    else:
        raise RuntimeError('ERROR: Unable to find bash')

    return path



def runshell(cmd, folder, arg = "", getOutput=True, echoCmd=True, fatal=True, onError=None):
    if isWindows:
        bash = getBashPath()
        if not bash:
            raise RuntimeError(
                "ERROR: Unable to find bash.exe, needed for running {}", cmd
            )

        if os.path.isabs(cmd):
            cmd = dos2bashPath(cmd)
        d = dos2bashPath(folder)
        cmd = '"{}" -l -c "cd {} && {} {}"'.format(bash, d, cmd, arg)
    else:
        cmd = "%s %s" % (self.command, arg)

    return runcmd(cmd)


MSVCinfo = None


def getMSVCInfo(PYTHON, arch, set_env=False):
    """
    Fetch info from the system about MSVC, such as versions, paths, etc.
    """
    global MSVCinfo
    if MSVCinfo is not None:
        return MSVCinfo

    # from attrdict import AttrDict

    # Note that it starts with a monkey-patch in setuptools.msvc to
    # workaround this issue: pypa/setuptools#1902
    cmd = (
        "import os, sys, setuptools.msvc; "
        "setuptools.msvc.isfile = lambda path: path is not None and os.path.isfile(path); "
        "ei = setuptools.msvc.EnvironmentInfo('{}', vc_min_ver=14.0); "
        "env = ei.return_env(); "
        "env['vc_ver'] = ei.vc_ver; "
        "env['vs_ver'] = ei.vs_ver; "
        "env['arch'] = ei.pi.arch; "
        "env['py_ver'] = sys.version_info[:2]; "
        "print(env)"
    )
    cmd = cmd.format(arch)
    env = eval(runcmd('"%s" -c "%s"' % (PYTHON, cmd), getOutput=True, echoCmd=True))

    from attrmap import AttrMap

    info = AttrMap(env)

    if set_env:
        os.environ["PATH"] = info.path
        os.environ["INCLUDE"] = info.include
        os.environ["LIB"] = info.lib
        os.environ["LIBPATH"] = info.libpath

        # We already have everything we need, tell distutils to not go hunting
        # for it all again if it happens to be called.
        os.environ["DISTUTILS_USE_SDK"] = "1"
        os.environ["MSSdk"] = "1"

    MSVCinfo = info
    return info


def getMinGW64():
    # config.guess is the same folder
    arch = runshell(
            os.path.dirname(__file__) + "./../platform-agnostic/" + "./config.guess",
            os.path.dirname(__file__),
            getOutput=True,
            echoCmd=True)
    # Show arch
    print(arch)

if __name__ == "__main__":
    if not isWindows:
        print('This python is for Windows specifically')
        sys.exit(-1)

    bashPath = getBashPath()
    print(bashPath)
    filePath = dos2bashPath(os.path.dirname(__file__))
    print(filePath)
    getMinGW64()

    PYTHON = sys.executable
    getMSVCInfo(PYTHON, "x64", set_env=True)
    print(MSVCinfo)
