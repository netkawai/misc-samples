# I always forget how to enable wireless through systemd-netowrkd. so I keep simple script for my memo
# I assume that wpa_supplicant, systemd-networkd and systemd-resolved were already installed
# Use relative path, I do not write sudo

if "$#" < 1; then
 echo 'Please specify PSK password'
 exit(-1)
fi

# copy wpa_suplicant.conf with a correct interface name (wlp4s0 should match the interface name)
cp ./wpa_supplicant.conf /etc/wpa_supplicant/wpa_supplicant-wlp8s0.conf
sed 's/XXX/"$1"/g' /etc/wpa_supplicant/wpa_supplicant-wlp8s0.conf
systemctl enable wpa_supplicant@wlp8s0.service

cp ./20-wireless.network /etc/systemd/network/20-wlp8s0.network
systemctl enable systemd-networkd.service
systemctl reload systemd-networkd.service

# Start WPA first
systemctl start wpa_supplicant@wlp8s0.serivce
# Start systemd-networkd and systemd-resolved
systemctl start systemd-networkd
systemctl start systemd-resolved

echo "Please reboot system to enable wireless"
