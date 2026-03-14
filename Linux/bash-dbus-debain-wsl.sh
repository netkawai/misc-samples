set -x
echo $BASHPID
export XDG_RUNTIME_DIR=/run/user/$(id -u)
if [ ! -d "$XDG_RUNTIME_DIR" ]; then
{
  sudo mkdir $XDG_RUNTIME_DIR && sudo chmod 700 $XDG_RUNTIME_DIR && sudo chown $(id -un):$(id -gn) $XDG_RUNTIME_DIR
  # System D-Bus (Debian/Ubuntu can install legacy service command start dbus without systemd (PID1))
  sudo service dbus start
}
fi

set_session_dbus()
{
    local bus_file_path="$XDG_RUNTIME_DIR/bus"

    export DBUS_SESSION_BUS_ADDRESS=unix:path=$bus_file_path
    if [ ! -e "$bus_file_path" ]; then
    {
        /usr/bin/dbus-daemon --session --address=$DBUS_SESSION_BUS_ADDRESS --nofork --nopidfile --syslog-only &

        # --------------------
        # More background processes can be added here.
        # For 'sudo' requiring commands, you should add them above
        # where the 'dbus' service is started.
        # --------------------

    }
    fi
}

set_session_dbus
set +x
