# force not to start x11 (only wayland)
#gnome-shell --wayland --no-x11
# start gnome-session (with autostart xwayland in default)
# dbus-run-session was deprecated 
if [[ $(tty) == /dev/tty1 && $XDG_SESSION_TYPE == wayland ]]; then
  gnome-session --no-reexec
else
  echo "Please create /etc/systemd/system/getty@tty1.service.d/wayland.conf"
  echo "[Service]"
  echo "Environment=XDG_SESSION_TYPE=wayland"
fi
# stop automatic sleep
# gsettings set org.gnome.settings-daemon.plugins.power sleep-inactive-ac-timeout 0

