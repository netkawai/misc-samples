# force not to start x11 (only wayland)
#gnome-shell --wayland --no-x11
# start gnome-session (with autostart xwayland in default)
dbus-run-session -- gnome-shell  --wayland --nested --no-x11
# stop automatic sleep
# gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-timeout 0

# does not work
#gsettings set org.gnome.mutter.experimental-features "['autostart-xwayland']" 1
