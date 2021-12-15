Install Python from Microsoft Store for Windows S and VS Code

1. Make sure App Execution Alias for Python.exe is enable (Optional)
2. Type python.exe in cmd.exe or Open Microsoft Store to install Python package
3. (Important) Open cmd.exe in user home (under %USERPROFILE%), python -m venv .venv
   NOTE: This is important since packaged python site libraries will be installed non accessible folder. you need Virtual Environment before doing anything.
4. Open the Environment variable for the user
5. Open Path variable for the user and add %USERPROFILE%\.venv\Scripts for Path
5. (Important) Disable all pip/python relating App Execution Alias
   NOTE: Otherwise, we cannot run simply Virtual Environment.
6. (Important) Sign out and sign in. Unlike Global Path variable, without signing out. The user level Environment variable will not apply
