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
1. The [ ] construct
To use -n -z, using [] construct requires double-quote
OK
```
str=""
if [ -z "$str" ]; then
  echo "The string is empty or null"
else
  echo "The string is not empty or null"
fi        
```
Not work
```
str=""
if [ -z $str ]; then
  echo "The string is empty or null"
else
  echo "The string is not empty or null"
fi        
```

The advantage of using these operators is that they are portable and easy to use. The disadvantage is that they may not work properly if the string contains spaces, tabs, or other special characters, unless you quote the string with double quotes.

2. The [[ ]] construct
Another way to check if a string is empty or null is to use the [[ ]] construct, which is a bash extension that provides more features and flexibility than the [ ] construct. The [[ ]] construct also supports the -z and -n operators, but it does not require quoting the string, and it can handle spaces and special characters better. For example, you can use the [[ ]] construct like this:

```
str=" "
if [[ -z $str ]]; then
  echo "The string is empty or null"
else
  echo "The string is not empty or null"
fi        
```
The advantage of using the [[ ]] construct is that it is more robust and reliable than the [ ] construct. The disadvantage is that it is not compatible with all shells, and it may not work in some older or non-standard systems.
