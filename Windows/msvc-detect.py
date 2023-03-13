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

runSilently = False


def dos2bashPath(path):
    """
    Convert an absolute dos-style path to one bash.exe can understand.
    """
    path = path.replace('\\', '/')
    cygpath = shutil.which('cygpath')
    wsl = shutil.which('wsl')

    # If we have cygwin then we can use cygpath to convert the path.
    # Note that MSYS2 (and Git Bash) now also have cygpath so this should
    # work there too.
    if cygpath:
        path = runcmd('"{}" -u "{}"'.format(cygpath, path), getOutput=True, echoCmd=False)
        return path
    elif wsl:
        # Are we using Windows System for Linux? (untested)
        path = runcmd('"{}" wslpath -a -u "{}"'.format(wsl, path), getOutput=True, echoCmd=False)
        return path
    else:
        # Otherwise, do a simple translate and hope for the best?
        # c:/foo --> /c/foo
        # TODO: Check this!!
        drive, rest = os.path.splitdrive(path)
        path = '/{}/{}'.format(drive[0], rest)
        return path
        
def msg(text):
    if not runSilently:
        print(text)


def runcmd(cmd, getOutput=False, echoCmd=True, fatal=True, onError=None):
    """
    Runs a give command-line command, optionally returning the output.
    """
    if isinstance(cmd, list):
        # add quotes to elements of the command that need it
        cmd = cmd[:]
        for idx, item in enumerate(cmd):
            if ' ' in item or '\t' in item or ';' in item:
                if item[0] not in ['"', "'"]:
                    if '"' in item:
                        item = item.replace('"', '\\"')
                    item = '"{}"'.format(item)
                    cmd[idx] = item
        # convert the resulting command to a string
        cmd = ' '.join(cmd)

    if echoCmd:
        msg(cmd)

    otherKwArgs = dict()
    if getOutput:
        otherKwArgs = dict(stdout=subprocess.PIPE,
                           stderr=subprocess.STDOUT)

    sp = subprocess.Popen(cmd, shell=True, env=os.environ, **otherKwArgs)

    output = None
    if getOutput:
        outputEncoding = 'cp1252' if sys.platform == 'win32' else 'utf-8'
        output = sp.stdout.read()
        output = output.decode(outputEncoding)
        output = output.rstrip()

    rval = sp.wait()
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

MSVCinfo = None
def getMSVCInfo(PYTHON, arch, set_env=False):
    """
    Fetch info from the system about MSVC, such as versions, paths, etc.
    """
    global MSVCinfo
    if MSVCinfo is not None:
        return MSVCinfo

    #from attrdict import AttrDict

    # Note that it starts with a monkey-patch in setuptools.msvc to
    # workaround this issue: pypa/setuptools#1902
    cmd = \
        "import os, sys, setuptools.msvc; " \
        "setuptools.msvc.isfile = lambda path: path is not None and os.path.isfile(path); " \
        "ei = setuptools.msvc.EnvironmentInfo('{}', vc_min_ver=14.0); " \
        "env = ei.return_env(); " \
        "env['vc_ver'] = ei.vc_ver; " \
        "env['vs_ver'] = ei.vs_ver; " \
        "env['arch'] = ei.pi.arch; " \
        "env['py_ver'] = sys.version_info[:2]; " \
        "print(env)"
    cmd = cmd.format(arch)
    env = eval(runcmd('"%s" -c "%s"' % (PYTHON, cmd), getOutput=True, echoCmd=True))

    from attrmap import AttrMap

    info = AttrMap(env)

    if set_env:
        os.environ['PATH'] =    info.path
        os.environ['INCLUDE'] = info.include
        os.environ['LIB'] =     info.lib
        os.environ['LIBPATH'] = info.libpath

        # We already have everything we need, tell distutils to not go hunting
        # for it all again if it happens to be called.
        os.environ['DISTUTILS_USE_SDK'] = "1"
        os.environ['MSSdk'] = "1"

    MSVCinfo = info
    return info

if __name__ == '__main__':
    bashPath = dos2bashPath('C:\\Users\\maka')
    print(bashPath)
    PYTHON = sys.executable
    getMSVCInfo(PYTHON, 'x64', set_env=True)
    print(MSVCinfo)
    