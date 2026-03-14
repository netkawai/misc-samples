# https://askubuntu.com/questions/923620/how-to-list-drivers-kernel-modules-affected-by-secureboot#:~:text=All%20kernel%20modules%20that%20are,t%20have%20the%20magic%20string.
find /lib/modules/5.15.0-91-generic -name '*.ko' -exec grep -FL '~Module signature appended~' {} \+
