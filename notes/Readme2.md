Windows NT 3.1 Installer disk
from:
[ProcessorVar] STF_PROCESSOR = "" ? $(!LIBHANDLE) GetProcessor
to:
[ProcessorVar] STF_PROCESSOR = $(ProcessorID_I586)
I386/INITIAL.IN_



Installer development

Open Source

WiX (backed by the former Microsoft Employee)
Pros
Some microsoft project is using
Cons
Not so active development, shift to MSIX package tool
Depends on .NET 3.5


Null Software(NSIS)
Pros
Complete open source
Cons
It is not so sophicate of script.
no validation script, you need a lot of tests

ClickOne
Cons
No update

Squirrel
Pros
Still active development
Can install any Windows 7, 8, 10
Super flexible, because you can develop your own installer easily.
Cons
All Wizard must be designed by ourself
Just library
Package technogology is actualy nuget.

MSIX(APPX)
Pros
Microsoft next generation offical package file format

Cons
Basically for Windows 10 only
In development for other Windows 7, 8.1 or they will support?????
Package signing is mandatory (however, most commerial version InstallShield, InstallAware provide their certificates)

