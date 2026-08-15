#!/bin/bash
export GDK_BACKEND=wayland
export LIBVA_DRIVER_NAME=i965

# 1. Capture the input argument, default to "gemini" if left blank
CHOICE=$(echo "${1:-gemini}" | tr '[:upper:]' '[:lower:]')

# 2. Input Validation Matrix: Map keywords to proper targets or quit
case "$CHOICE" in
  "gemini")
    TARGET_URL="https://gemini.google.com"
    ;;
  "youtube")
    TARGET_URL="https://youtube.com"
    ;;
  "google")
    TARGET_URL="https://www.google.com"
    ;;
  *)
    echo "ERROR: Invalid application target '$1'."
    echo "Usage: ./launch_app.sh [gemini | youtube | google]"
    exit 1
    ;;
esac

# 3. Setup distinct profile workspace
PROFILE_DIR="$HOME/.config/mini_apps/$CHOICE"

# 4. Global Baseline Flags (Includes Modern Intel/Wayland GPU Pipeline Fixes)
BASE_FLAGS=(
  --ozone-platform=wayland
  --ozone-platform-hint=auto
  --app="$TARGET_URL"
  --user-data-dir="$PROFILE_DIR"
  --no-first-run
  --no-default-browser-check
  --password-store=basic
  --js-flags="--max-old-space-size=8192 --max-semi-space-size=256"
  --ignore-gpu-blocklist
  --use-webgpu-adapter=opengles 
  --disable-vulkan-surface
  --disable-features=UseChromeOSDirectVideoDecoder,VaapiVideoEncoder
  --disable-accelerated-video-encode
  --disable-features=AcceleratedVideoDecodeLinuxZeroCopyGL
  --disable-features=VaapiVideoDecodeLinuxGLNext

  # Force layout drawing entirely to the CPU to bypass sluggish Ivy Bridge shader translation
  --disable-gpu-rasterization
  --disable-oop-rasterization
  --disable-skia-graphite
)

# 5. Routing Profiles
if [ "$CHOICE" = "youtube" ]; then
  echo "Launching YouTube with Permanent H.264 Enforcement..."
  
  # Purge old corrupt media buffers
  rm -rf "$PROFILE_DIR/Default/Cache"
  rm -rf "$PROFILE_DIR/Default/Code Cache"
  rm -rf "$PROFILE_DIR/Default/GPUCache"

  # Find the exact folder where this launch script is located
  SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
  EXT_DIR="$SCRIPT_DIR/enhanced-h264ify"
  
  # Fetch the extension to the script's directory permanently if missing
  if [ ! -d "$EXT_DIR" ]; then
    echo "Installing enhanced-h264ify extension inside $SCRIPT_DIR..."
    mkdir -p "$EXT_DIR"
    
    # Download the correct release archive directly from GitHub
    curl -L -o "$SCRIPT_DIR/h264-temp.zip" https://github.com/alextrv/enhanced-h264ify/archive/refs/heads/master.zip
    unzip -q "$SCRIPT_DIR/h264-temp.zip" -d "$SCRIPT_DIR/"
    
    # Move files out of the unzipped master folder directly into our target folder
    mv "$SCRIPT_DIR/enhanced-h264ify-master"/* "$EXT_DIR/"
    
    # Clean up the installation files
    rm -rf "$SCRIPT_DIR/h264-temp.zip" "$SCRIPT_DIR/enhanced-h264ify-master"
    echo "Extension installation complete."
  fi

  # Boot Chromium with the extension loaded from the script directory
  chromium "${BASE_FLAGS[@]}" \
    --load-extension="$EXT_DIR" \
    --disable-features=AV1Decoder,VpxVideoDecoder

elif [ "$CHOICE" = "google" ]; then
  # --- TARGETED SYSTEM POLICY CHECK ---
  SYS_POLICY_DIR="/etc/chromium/policies/managed"
  SYS_POLICY_FILE="$SYS_POLICY_DIR/adguard_doh.json"

  if [ ! -f "$SYS_POLICY_FILE" ]; then
    echo "[!] System policy missing. Authorizing sudo to install strict AdGuard configuration..."
    sudo mkdir -p "$SYS_POLICY_DIR"
# Replace DoH URL, if you have own private DoH
    sudo tee "$SYS_POLICY_FILE" > /dev/null << 'EOF'
{
  "BuiltInDnsClientEnabled": true,
  "DnsOverHttpsMode": "secure",
  "DnsOverHttpsTemplates": "https://family.adguard-dns.com/dns-query"
}
EOF
    echo "[+] System policy applied successfully. Continuing execution..."
  fi

  echo "Launching Google Search with Hardened Enterprise Policy DoH & Window Interceptor..."
  
  # Extension setup for layout/window lock management
  EXT_DIR="$PROFILE_DIR/extensions/tab-lock"
  mkdir -p "$EXT_DIR"

  cat << 'EOF' > "$EXT_DIR/manifest.json"
{
  "manifest_version": 3,
  "name": "Force Same Window Navigation",
  "version": "1.0",
  "permissions": ["tabs", "webNavigation"],
  "background": { "service_worker": "background.js" },
  "content_scripts": [{
    "matches": ["<all_urls>"],
    "js": ["content.js"],
    "run_at": "document_start",
    "all_frames": true
  }]
}
EOF

  cat << 'EOF' > "$EXT_DIR/content.js"
function patchLinks() {
  document.querySelectorAll('a[target]').forEach(link => link.removeAttribute('target'));
}
const observer = new MutationObserver(() => patchLinks());
observer.observe(document.documentElement, { childList: true, subtree: true });
window.addEventListener('DOMContentLoaded', patchLinks);

if (window.top === window.self) {
  const originalOpen = window.open;
  window.open = function(url, target, features) {
    if (url) { window.location.href = url; return window; }
    return originalOpen(url, target, features);
  };
}
EOF

  cat << 'EOF' > "$EXT_DIR/background.js"
chrome.tabs.onCreated.addListener((newTab) => {
  if (newTab.openerTabId) {
    chrome.tabs.onUpdated.addListener(function listener(tabId, changeInfo) {
      if (tabId === newTab.id && changeInfo.url) {
        chrome.tabs.onUpdated.removeListener(listener);
        chrome.tabs.update(newTab.openerTabId, { url: changeInfo.url });
        chrome.tabs.remove(newTab.id);
      }
    });
  }
});
EOF

  chromium "${BASE_FLAGS[@]}" \
    --load-extension="$EXT_DIR"

else
  echo "Launching Gemini in Windowed App Mode..."
  chromium "${BASE_FLAGS[@]}" \
    --disable-extensions
fi
