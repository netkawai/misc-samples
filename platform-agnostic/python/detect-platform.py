import fnmatch
import glob
import json
import optparse
import os
import platform
import random
import re
import string
import subprocess
import sys
import time
from datetime import datetime, timedelta

from optparse import OptionGroup
from optparse import OptionParser
from sys import stderr
from sys import stdout

def printf(format, *args):
    sys.stdout.write(format % args)

class AbortError( Exception ):
    def __init__( self, format, *args ):
        self.value = format % args
    def __str__( self ):
        return self.value


###############################################################################
##
## abstract action
##
## pretext = text which immediately follows 'probe:' output prefix
## abort   = if true configure will exit on probe fail
## head    = if true probe session is stripped of all but first line
## session = output from command, including stderr
## fail    = true if probe failed
##
class Action( object ):
    actions = []

    def __init__( self, category, pretext='unknown', abort=False, head=False ):
        if self not in Action.actions:
            Action.actions.append( self )

        self.category = category
        self.pretext  = pretext
        self.abort    = abort
        self.head     = head
        self.session  = None

        self.run_done = False
        self.fail     = True
        self.msg_fail = 'fail'
        self.msg_pass = 'pass'
        self.msg_end  = 'end'

    def _actionBegin( self ):
        printf( '%s: %s...', self.category, self.pretext )

    def _actionEnd( self ):
        if self.fail:
            printf( '(%s) %s\n', self.msg_fail, self.msg_end )
            if self.abort:
                #self._dumpSession( cfg.infof )
                printf( 'unable to continue' )
            #self._dumpSession( cfg.verbosef )
            #self._failSession()
        else:
            printf( '(%s) %s\n', self.msg_pass, self.msg_end )
            #self._dumpSession( cfg.verbosef )

    def _dumpSession( self, printf ):
        if self.session and len(self.session):
            for line in self.session:
                printf( '  : %s\n', line )
        else:
            printf( '  : <NO-OUTPUT>\n' )

    def _parseSession( self ):
        pass

    def _failSession( self ):
        pass

    def run( self ):
        if self.run_done:
            return
        self.run_done = True
        self._actionBegin()
        self._action()
        if not self.fail:
            self._parseSession()
        self._actionEnd()



###############################################################################
##
## base probe: anything which runs in shell.
##
## pretext = text which immediately follows 'probe:' output prefix
## command = full command and arguments to pipe
## abort   = if true configure will exit on probe fail
## head    = if true probe session is stripped of all but first line
## session = output from command, including stderr
## fail    = true if probe failed
##
class ShellProbe( Action ):
    def __init__( self, pretext, command, abort=False, head=False ):
        super( ShellProbe, self ).__init__( 'probe', pretext, abort, head )
        self.command = command

    def _action( self ):
        ## pipe and redirect stderr to stdout; effects communicate result
        pipe = subprocess.Popen( self.command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT )

        ## read data into memory buffers, only first element (stdout) data is used
        data = pipe.communicate()
        self.fail = pipe.returncode != 0

        if data[0]:
            self.session = data[0].splitlines()
        else:
            self.session = []

        if pipe.returncode:
            self.msg_end = 'code %d' % (pipe.returncode)

    def _dumpSession( self, printf ):
        printf( '  + %s\n', self.command )
        super( ShellProbe, self )._dumpSession( printf )


###############################################################################
##
## GNU host tuple probe: determine canonical platform type
##
## example results from various platforms:
##
##   powerpc-apple-darwin9.6.0  (Mac OS X 10.5.6 PPC)
##   i386-apple-darwin9.6.0     (Mac OS X 10.5.6 Intel)
##   x86_64-apple-darwin10.8.0  (Mac OS X 10.6.8 Intel)
##   x86_64-apple-darwin11.2.0  (Mac OS X 10.7.2 Intel)
##   i686-pc-cygwin             (Cygwin, Microsoft Vista)
##   x86_64-unknown-linux-gnu   (Linux, Fedora 10 x86_64)
##


class HostTupleProbe( ShellProbe, list ):
    GNU_TUPLE_RE = '([^-]+)-?([^-]*)-([^0-9-]+)([^-]*)-?([^-]*)'

    def __init__( self ):
        super( HostTupleProbe, self ).__init__( 'host tuple', './config.guess', abort=True, head=True )

    def _parseSession( self ):
        self.spec = self.session[0] if self.session else ''

        ## grok GNU host tuples
        m = re.match( HostTupleProbe.GNU_TUPLE_RE, self.spec )
        if not m:
            self.fail = True
            self.msg_end = 'invalid host tuple: %s' % (self.spec)
            return

        self.msg_end = self.spec

        ## assign tuple from regex
        self[:] = m.groups()

        ## for clarity
        self.machine = self[0]
        self.vendor  = self[1]
        self.system  = self[2]
        self.release = self[3]
        self.extra   = self[4]

        ## nice formal name for 'system'
        self.systemf = platform.system()

        if self.match( '*-*-cygwin*' ):
            self.systemf = self[2][0].upper() + self[2][1:]

    ## glob-match against spec
    def match( self, *specs ):
        for spec in specs:
            if fnmatch.fnmatch( self.spec, spec ):
                return True
        return False
try:
    host  = HostTupleProbe(); host.run()


    printf(host.spec + '\n')
    printf(host.machine + '\n')
    printf(host.vendor + '\n')
    printf(host.system + '\n')
    printf(host.systemf + '\n')
    printf(host.release + '\n')
    printf(host.extra + '\n')
   # printf('%s %s' % (host.systemf,arch.mode.default) )

except AbortError, x:
    stderr.write( 'ERROR: %s\n' % (x) )
    sys.exit( 1 )

sys.exit( 0 )
