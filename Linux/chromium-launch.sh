#!/bin/bash
export GDK_BACKEND=wayland
export LIBVA_DRIVER_NAME=i965

TARGET_URL=${1:-"https://gemini.google.com"}
APP_NAME=$(echo "$TARGET_URL" | awk -F[/.] '{print $3}')
PROFILE_DIR="$HOME/.config/mini_apps/$APP_NAME"

# 1. Base flags optimized for maximum CPU throughput and speed
BASE_FLAGS=(
  --ozone-platform=wayland
  --app="$TARGET_URL"
  --user-data-dir="$PROFILE_DIR"
  --no-first-run
  --no-default-browser-check
  --password-store=basic
  
  # Maximize JavaScript heap size & expand temporary memory buffers
  # This stops the Garbage Collector from repeatedly breaking the CPU pipeline
  --js-flags="--max-old-space-size=2048 --max-semi-space-size=128"
)

if [[ "$TARGET_URL" == *"youtube.com"* ]]; then
  echo "Launching YouTube with Accelerated Video and High-Throughput CPU Profiling..."
  
#    --load-extension="$HOME/h264_extension" \
  chromium "${BASE_FLAGS[@]}" \
    --enable-features=VaapiVideoDecoder,VaapiVideoEncoder,VaapiIgnoreDriverChecks \
    --ignore-gpu-blocklist \
    --use-gl=desktop \
    --blink-settings=preferredColorScheme=1 \
    --disable-features=WebRtcMediaStreamTrackRemoteVideoDecoders,AV1Decoder \
    --user-agent="Mozilla/5.0 (Apple TV; CPU OS 12_0 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/12.0 Mobile/15E148 Safari/604.1"

else
  echo "Launching $APP_NAME with Maximum JIT Optimization Speeds..."
  
  # For Gemini/Search, we leave hardware acceleration enabled for general rendering 
  # but allow the CPU compilation pipeline to run unrestricted.
  chromium "${BASE_FLAGS[@]}" \
    --ignore-gpu-blocklist \
    --use-gl=desktop
fi
