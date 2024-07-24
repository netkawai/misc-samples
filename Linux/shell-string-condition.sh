# dash (busybox) this is NG
val = "NONE"
checkVal = $(echo -n $val | grep -q "*UPDATE")
# NG NG
if [ -n $checkVal ]; then
   echo "UPDATE icnlude false statement"
fi
# this is OK
if [ -n "$checkVal" ]; then
   echo "UPDATE icnlude this will not appear"
fi
