#!/usr/bin/env bash

#!/usr/bin/env bash

# Configurations
WALLPAPER_DIR="$HOME/.cache/wallpaper"
TARGET_IMG="$WALLPAPER_DIR/daily_verse.jpg"
TARGET_URL="https://dailyverses.net"

# Create storage directory if missing
mkdir -p "$WALLPAPER_DIR"

# 1. Fetch the raw HTML spoofing a browser and isolate the specific verse-named image path 
# This looks for the primary preview meta image (e.g., romans-8-1-2.jpg)
IMG_URL=$(curl -sL -A "Mozilla/5.0 (X11; Linux x86_64)" "$TARGET_URL" | grep -oP '<meta property="og:image" content="\K[^"]+')

# 2. Fallback parse step if the website meta template layout changes unexpectedly
if [ -z "$IMG_URL" ]; then
    IMG_URL=$(curl -sL -A "Mozilla/5.0 (X11; Linux x86_64)" "$TARGET_URL" | grep -oP 'src="\Khttps://dailyverses\.net/images/[^"]+\.jpg' | head -n 1)
fi

# 3. Action the file retrieval if an image string was isolated
if [ -n "$IMG_URL" ]; then
    echo "Identified Daily Verse Image Path: $IMG_URL"
    curl -sL -A "Mozilla/5.0 (X11; Linux x86_64)" -o "$TARGET_IMG" "$IMG_URL"
else
    echo "Parsing error: Unable to detect any verse-named image layout on the homepage."
fi

# 4. Bind the image asset using your Wayland wallpaper manager
if [ -f "$TARGET_IMG" ]; then
    pkill -x wbg
    # pkill -x swaybg
    
    # Run wbg asynchronously 
    ~/wbg/build/wbg "$TARGET_IMG" &
else
    echo "Error: High-resolution verse image asset could not be verified locally."
fi

