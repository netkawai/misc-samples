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

Checkbox for markdown

- [ ] A
- [ ] B
- [ ] C


Leader: Alexander (356BC) - Julius/Octavian
Language: Greek
Belief: Judaism - early Christian


Leader: Ashoka (304BC)
Language: Proto-Sanskrit(Bhrahmic)
Belief: Hinduism - Buddhism


Leader: Zheng(259BC,Qin) - Liu(256BC, Han)
Language: Han(Chinese)
Belief: Confucianism


Leader: Muhammad(570CE)
Language: Arabic
Belief: Islam


Leader: Charlemagne(748CE)
Language: Colloquial
Belief: Christianity, Renaissance




Calculation for skews for two lines

Line a
Line b
[Vector.Norm = sqrt(x*x+y*y+z*z)]
Vector M = a.direction.Cross(b.direction)

If M.Norm < Tolerance then coplanar

Vector R = (1.0/ (M.Norm)^2) * (a.Point - b.Point).Cross(M)

Return (b.Direction.Dot(R), a.Direction.Dot(R))


https://research.hackerrank.com/developer-skills/2018

uvicorn default 8000
npm run serve 8080


International yard     0.9144 exactly
Imperial yard(UK-past) 0.9143969 exactly
US Survey yard(USA)    3600/3937 exactly

International pound
0.45359237 kilograms
0.45359237

(0.3048/12.0) == 0.0254 NG
(3.048/120) == 0.0254 OK
(1.1+0.11) == 1.21 NG
(11+1.1) == 12.1 OK
