#!/bin/sh
# without bash.. oh. no.
input=$(ls /var/log/log*.txt)
input=$(echo $input | tr "\n" " ")
echo "input: $input"
logCount=0
firstOne=
OLDIFS=$IFS;IFS=' '
for token in $input; do
    echo "token: ${token%:*}"
    if [ -z ${firstOne} ]; then
      firstOne=${token%:*}
    fi
    logCount=$((logCount+1))    
done
IFS=$OLDIFS
echo "first:$firstOne"
echo $logCount
