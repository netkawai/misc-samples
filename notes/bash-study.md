# bash redirect.
# redirect stdout and stderr into one file
# here is a file contain text I am file content
```bash
$ ls
here
$ cat here
I am file content
$ cat here file2 > log.txt 2>&1
$ cat log.txt
I am file content
cat: file2: No such file or directory
$ rm log.txt
$ cat here file2 > (tee log.txt) 2>&1
bash: syntax error near unexpected token `('  <= why?????
$ cat here file2 > >(tee log.txt) 2>&1
I am file content
cat: file2: No such file or directory
$ cat log.txt
I am file content
cat: file2: No such file or directory
```
