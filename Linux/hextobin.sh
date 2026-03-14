hex_string="48656c6c6f20576f726c64"
printf "$(sed 's/../\\x&/g' <<< "$hex_string")" > output.bin