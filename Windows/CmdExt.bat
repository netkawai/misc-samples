REM Came from https://www.itprotoday.com/compute-engines/jsi-tip-7136-how-can-i-use-script-test-if-command-extensions-are-enabled
REM They have this copyright.
@echo off
if \{%1\}==\{\} @echo Syntax CMDExt Status&goto :EOF
setlocal
set Ext=N
call :HKCU>nul 2>&1
if /i "%Ext%" NEQ "EnableExtensions" goto chkHKLM
if /i "%ExtV%" EQU "0x1" set Ext=Y&goto OC
set Ext=N
goto OC
:HKCU
for /f "Skip=4 Tokens=1-3" %%e in ('reg query "HKCU\Software\Microsoft\Command Processor" /v EnableExtensions') do set Ext=%%e&set ExtV=%%g
goto :EOF
:HKLM
set Ext=N
for /f "Skip=4 Tokens=1-3" %%e in ('reg query "HKLM\Software\Microsoft\Command Processor" /v EnableExtensions') do set Ext=%%e&set ExtV=%%g
goto :EOF
:chkHKLM
call :HKLM>nul 2>&1
if /i "%Ext%" NEQ "EnableExtensions" set Ext=N&goto OC
if /i "%ExtV%" EQU "0x1" set Ext=Y&goto OC
set EXT=N
:OC
for /f "Tokens=*" %%e in ('@echo %CMDCMDLINE%') do set ExtV=%%e
set work=%ExtV:/E:ON=%
if "%work%" NEQ "%ExtV%" set Ext=Y&goto isON
set work=%ExtV:/X=%
if "%work%" NEQ "%ExtV%"  set Ext=Y&goto isON
set work=%ExtV:/E:OFF=%
if  "%work%" NEQ "%ExtV%" set Ext=N&goto OFF
set work=%ExtV:/Y=%
if  "%work%" NEQ "%ExtV%"  set Ext=N&goto OFF
:isON
set work=%ExtV:/E:OFF=%
if "%work%" NEQ "%ExtV%" set Ext=N&goto OFF
set work=%ExtV:/Y=%
if "%work%" NEQ "%ExtV%"  set Ext=N&goto OFF
if "%Ext%" NEQ "Y" goto OFF
:ON
VERIFY OTHER 2>nul
SETLOCAL ENABLEEXTENSIONS
IF ERRORLEVEL 1 set Ext=N
:OFF
endlocal&set %1=%Ext%
