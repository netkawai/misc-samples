# This is disable include disable

systemctl stop systemd-resolved
# If you have D-bus desktop, probably you have socket
systemctl stop systemd-networkd
systemctl stop wpa_supplicant@wlp8s0.service

# At least disable wpa_supplicant to prevent grab wifi device
systemctl disable wpa_supplicant@wlp8s0.service
# You can remove
rm /etc/wpa_supplicant/wpa_supplicant-wlp8s0.conf
rm /etc/systemd/network/10-wlp8s0.network

# Do I need to disable networkd or resolved? unless switch to network-manager

