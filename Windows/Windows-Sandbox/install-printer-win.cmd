REM Install virtual printer
rundll32 printui.dll,PrintUIEntry /if  /f %windir%\inf\ntprint.inf /r "lpt1:" /m "Microsoft MS-XPS Class Driver 2"
