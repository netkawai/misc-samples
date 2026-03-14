https://stackoverflow.com/questions/71871664/python-slash-testing-framework-and-vscode-debugging
I found the solution for this.

Just create this launch.json inside the .vscode folder
```
{
      "version": "0.2.0",
      "configurations": [
      {
         "name": "slash testing",
         "type": "python",
         "request": "launch",
         "module": "slash.frontend.main",
         "args": [
             "run",
             "-vvv"
         ],
         "console": "integratedTerminal",
         "justMyCode": false
      }
   ]
}
```
So I needed to change the module name.
