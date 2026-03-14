# busybox(sh) 
val = "NONE"
checkVal = $(echo -n $val | grep -q "*UPDATE")
# NG NG 
if [ -n $checkVal ]; then
   echo "UPDATE include false statement"
fi
# this is OK
if [ -n "$checkVal" ]; then
   echo "UPDATE icnlude this will not appear"
fi

# other case
# this is NG to show "ABC DEF"
val="ABC";val="$val"$'\n'"DEF";echo $val
# this is OK
# ABC
# DEF
val="ABC";val="$val"$'\n'"DEF";echo "$val"

# In shell double quuote is matter.
