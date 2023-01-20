sudo sed -i 's/^Exec=[^ ]*/& --no-x11/' /usr/share/applications/org.gnome.Shell.desktop
sudo mv /usr/lib/gsd-xsettings /usr/lib/disabled-gsd-xsettings
