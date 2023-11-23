#!/bin/sh
# without bash.. oh. no.
input=$(ls /var/log/log*.txt)
input=$(echo $input | tr "\n" " ")
echo "input: $input"
logCount=0
OLDIFS=$IFS;IFS=' '
for token in $input; do
    echo "token: ${token%:*}"
    logCount=$((logCount+1))
done
IFS=$OLDIFS
echo $logCount
