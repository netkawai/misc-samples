# If I need to use wire connection (ethernet), I should be sure wpa_supplication deamon is not running

cp ./10-wired.network /etc/systemd/network/10-enp7s0.network
systemctl enable systemd-networkd.service
systemctl reload systemd-networkd.service

# Start systemd-networkd and systemd-resolved
systemctl start systemd-networkd
systemctl start systemd-resolved
