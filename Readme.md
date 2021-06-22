This repository is miscellaneous, memos, and snippets.


If you copy any codes from here, I do not have any responsibility.
There are some codes from others. 
I simply copied and modified them. I do not declare copyright for those codes as well.

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