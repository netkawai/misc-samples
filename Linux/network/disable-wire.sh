# This is disable include disable

systemctl stop systemd-resolved
# If you have D-bus desktop, probably you have socket
systemctl stop systemd-networkd

rm /etc/systemd/network/10-enp7s0.network

# Do I need to disable networkd or resolved? unless switch to network-manager
# Force to down ethernet
ip link set enp7s0 down
