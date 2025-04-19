# force not to start x11 (only wayland)
#gnome-shell --wayland --no-x11
# start gnome-session (with autostart xwayland in default)
XDG_SESSION_TYPE=wayland dbus-run-session gnome-session
# stop automatic sleep
# gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-timeout 0

