#!/usr/bin/env bash

# Configurations
WALLPAPER_DIR="$HOME/.cache/wallpaper"
TARGET_IMG="$WALLPAPER_DIR/daily_verse.jpg"
TMP_IMG="$WALLPAPER_DIR/daily_verse.jpg.part"
WALLPAPER_BIN="$HOME/wbg/build/wbg"
TARGET_URL="https://dailyverses.net"
UA="Mozilla/5.0 (X11; Linux x86_64)"

# Create storage directory if missing
mkdir -p "$WALLPAPER_DIR"

# 0. Make sure the wallpaper binary can actually run before doing any network work.
#    A missing shared library (e.g. libjxl ABI bump after a package upgrade) makes
#    wbg exit 127 immediately, which silently leaves the screen with no background.
if ! "$WALLPAPER_BIN" --help >/dev/null 2>&1 && [ ! -x "$WALLPAPER_BIN" ]; then
    echo "Error: $WALLPAPER_BIN is not executable." >&2
    exit 1
fi
if ! ldd "$WALLPAPER_BIN" 2>/dev/null | grep -q "not found"; then
    : # all libraries resolve
else
    echo "Error: $WALLPAPER_BIN has unresolved libraries:" >&2
    ldd "$WALLPAPER_BIN" | grep "not found" >&2
    echo "Hint: rebuild it, e.g. (cd ~/wbg && meson setup build --reconfigure && ninja -C build)" >&2
    exit 1
fi

# 1. Fetch the raw HTML spoofing a browser and isolate the specific verse-named image path
# This looks for the primary preview meta image (e.g. romans-8-1-2.jpg)
IMG_URL=$(curl -sL -A "$UA" "$TARGET_URL" | grep -oP '<meta property="og:image" content="\K[^"]+')

# 2. Fallback parse step if the website meta template layout changes unexpectedly
if [ -z "$IMG_URL" ]; then
    IMG_URL=$(curl -sL -A "$UA" "$TARGET_URL" | grep -oP 'src="\Khttps://dailyverses\.net/images/[^"]+\.jpg' | head -n 1)
fi

# 3. Download to a temp file first, so a failed/partial fetch never destroys the
#    image currently being displayed.
if [ -n "$IMG_URL" ]; then
    echo "Identified Daily Verse Image Path: $IMG_URL"
    if curl -fL -A "$UA" -o "$TMP_IMG" "$IMG_URL" && [ -s "$TMP_IMG" ]; then
        mv -f "$TMP_IMG" "$TARGET_IMG"
    else
        rm -f "$TMP_IMG"
        echo "Warning: download failed; keeping previous wallpaper image." >&2
    fi
else
    echo "Parsing error: Unable to detect any verse-named image layout on the homepage." >&2
fi

# 4. Bind the image asset using your Wayland wallpaper manager.
#    wbg is a pure wlr-layer-shell client, so Xwayland does not affect it.
if [ -f "$TARGET_IMG" ]; then
    pkill -x wbg
    # wait for the previous instance to release the layer surface, otherwise the
    # new one can attach while the old one is still mapped on top of it.
    for _ in $(seq 20); do
        pgrep -x wbg >/dev/null || break
        sleep 0.1
    done

    # Run wbg asynchronously
    "$WALLPAPER_BIN" "$TARGET_IMG" &
    sleep 0.5
    if ! pgrep -x wbg >/dev/null; then
        echo "Error: wbg exited immediately; background image not applied." >&2
        exit 1
    fi
else
    echo "Error: High-resolution verse image asset could not be verified locally." >&2
    exit 1
fi
