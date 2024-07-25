So now, most modern glibc has locale both C.UTF-8 and C
Well, what are different. As the name implies, C.UTF-8 is POSIX C + UTF-8 (and by enlarge, Unicode rule).
For example
```
echo 'É' | LC_ALL=C.UTF-8 grep '[[:upper:]]'
É
```
On the other hand
```
echo 'É' | LC_ALL=C. grep '[[:upper:]]'
(no output)
```
However, some systems does not have enough storage, C.UTF-8 becomes alias of C... so still we need to check, really works or not.
