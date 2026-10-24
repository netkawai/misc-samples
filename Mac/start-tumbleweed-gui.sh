#!/bin/bash

# --- CONFIGURATION ---
CONTAINER_NAME="tumbleweed-labwc"
IMAGE_NAME="opensuse/tumbleweed:latest"
VNC_PORT="5900"
PASSWORD="123456"

echo "=== OpenSUSE Tumbleweed GUI Automated Launcher ==="

# 1. Check if the container image exists, if not pull it
if ! container image inspect $IMAGE_NAME >/dev/null 2>&1; then
    echo "[-] Image $IMAGE_NAME not found. Fetching..."
    container pull $IMAGE_NAME
fi

# 2. Check container status
CONTAINER_STATE=$(container list --all --format "{{.State}}" --filter "name=${CONTAINER_NAME}" 2>/dev/null)

if [ -z "$CONTAINER_STATE" ]; then
    echo "[+] Creating and starting a fresh container instance..."
    container run -d --name "$CONTAINER_NAME" "$IMAGE_NAME" sleep infinity
elif [ "$CONTAINER_STATE" != "running" ]; then
    echo "[+] Starting stopped container instance..."
    container start "$CONTAINER_NAME"
else
    echo "[*] Container is already running."
fi

# Give the microVM a brief window to initialize its network interface
sleep 2

# 3. Get the internal bridge IP address assigned by macOS
CONTAINER_IP=$(container list --filter "name=${CONTAINER_NAME}" --format "{{.IP}}")
if [ -z "$CONTAINER_IP" ]; then
    echo "[!] Error: Could not resolve the container's IP address."
    exit 1
fi
echo "[*] Target Container IP: $CONTAINER_IP"

# 4. Provision software dependencies inside Tumbleweed seamlessly
echo "[+] Ensuring GUI dependencies are installed (this skips if already present)..."
container exec "$CONTAINER_NAME" zypper --non-interactive install -y \
    labwc wayvnc foot adobe-sourcecodepro-fonts wl-clipboard mesa-dri >/dev/null 2>&1

# 5. Inject headless infrastructure configurations
echo "[+] Injecting display server profiles..."
container exec "$CONTAINER_NAME" mkdir -p /root/.config/wayvnc /root/.config/labwc

# Write configuration definitions
container exec "$CONTAINER_NAME" bash -c "cat << 'EOF' > /root/.config/wayvnc/config
address=0.0.0.0
port=$VNC_PORT
enable_auth=true
allow_broken_crypto=true
relax_encryption=true
password=$PASSWORD
EOF"

# Write keyboard alt+enter map profiles
container exec "$CONTAINER_NAME" bash -c "cat << 'EOF' > /root/.config/labwc/rc.xml
<?xml version=\"1.0\" encoding=\"utf-8\"?>
<labwc_config>
  <keyboard>
    <default />
    <keybind key=\"A-Return\">
      <action name=\"Execute\" command=\"foot\" />
    </keybind>
  </keyboard>
</labwc_config>
EOF"

# 6. Initialize the headless background graphic layers
echo "[+] Initializing background Wayland rendering pipeline..."
container exec -d "$CONTAINER_NAME" bash -c "
    export XDG_RUNTIME_DIR=/tmp/wayland-runtime
    mkdir -p \$XDG_RUNTIME_DIR
    chmod 700 \$XDG_RUNTIME_DIR
    export WLR_BACKENDS=headless
    export WAYLAND_DISPLAY=wayland-0
    
    # Kill stale instances if present to prevent port binds failing
    pkill -9 wayvnc 2>/dev/null
    pkill -9 labwc 2>/dev/null
    sleep 0.5
    
    # Launch compositor infrastructure and stream
    labwc -s 'wayvnc'
"

# Give the servers a moment to bind to the port
sleep 1.5

# 7. Auto-inject the first primary terminal canvas instance
container exec -d "$CONTAINER_NAME" env XDG_RUNTIME_DIR=/tmp/wayland-runtime WAYLAND_DISPLAY=wayland-0 foot

# right now no start client in script
exit 0

# 8. Fire up the native viewer connection string from macOS
echo "[+] Launching TigerVNC Viewer connection hook..."
if [ -d "/Applications/TigerVNC Viewer.app" ]; then
    open -a "TigerVNC Viewer" --args "$CONTAINER_IP:$VNC_PORT"
else
    echo "[!] TigerVNC Viewer app not found in /Applications."
    echo "[*] Handshake ready. You can manually point your VNC client to: $CONTAINER_IP:$VNC_PORT"
fi
