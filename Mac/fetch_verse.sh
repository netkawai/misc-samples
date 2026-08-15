#!/bin/zsh

# Configurations
WALLPAPER_DIR="$HOME/Library/Mobile Documents/com~apple~CloudDocs/Pictures/Slideshow"
TARGET_URL="https://dailyverses.net"
LAST_RUN_FILE="$HOME/.config/daily_verse_last_run"
TIMER_FILE="$HOME/.config/daily_verse_timer"
CURRENT_IMG_FILE="$HOME/.config/daily_verse_current_path"

mkdir -p "$HOME/.config"
mkdir -p "$WALLPAPER_DIR"

TODAY=$(date "+%Y-%m-%d")

# Function to EXPLICITLY set the macOS system backend back to a folder Shuffle
restore_shuffle() {
    echo "15-minute mark reached. Programmatically enforcing Folder Shuffle..."
    
    # This AppleScript tells macOS to stop using a static image, switch to the folder, and turn on random shuffle
    osascript -e "
    tell application \"System Events\"
        tell every desktop
            set picture rotation to 1 -- 1 means 'interval' rotation (Slideshow)
            set random order to true   -- Explicitly enables Random / Shuffle
            set change interval to 900  -- FIX: 900 seconds = exactly 15 minutes rotation
        end tell
    end tell"
    
    echo "Shuffle mode explicitly restored with a 15-minute rotation cycle."
}

# --- Handling Login / Logout Splits (Cumulative 15-minute Tracker) ---
if [[ -f "$LAST_RUN_FILE" ]]; then
    LAST_RUN=$(cat "$LAST_RUN_FILE")
    if [[ "$LAST_RUN" == "$TODAY" ]]; then
        if [[ -f "$TIMER_FILE" ]]; then
            TIME_SPENT=$(cat "$TIMER_FILE")
            if [[ "$TIME_SPENT" -ge 900 ]]; then
                echo "15 minutes already achieved today. Exiting."
                exit 0
            else
                echo "Resuming countdown. Accumulating time from $TIME_SPENT seconds..."
                if [[ -f "$CURRENT_IMG_FILE" ]]; then
                    TARGET_IMG=$(cat "$CURRENT_IMG_FILE")
                    osascript -e "tell application \"System Events\" to tell every desktop to set picture to \"$TARGET_IMG\""
                fi
            fi
        fi
    fi
fi

# --- If it's a brand new day, download the new file ---
if [[ "$LAST_RUN" != "$TODAY" ]]; then
    # Wait for network connection
    TIMEOUT=0
    while ! ping -c 1 -t 2 dailyverses.net > /dev/null 2>&1; do
        sleep 3
        ((TIMEOUT+=3))
        if [[ $TIMEOUT -ge 60 ]]; then
            echo "Network timeout."
            exit 1
        fi
    done

    # Fetch new image URL
    HTML_DATA=$(curl -sL -A "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7)" "$TARGET_URL")
    IMG_URL=$(echo "$HTML_DATA" | grep -o 'meta property="og:image" content="[^"]*' | awk -F'content="' '{print $2}')
    
    if [[ -z "$IMG_URL" ]]; then
        IMG_URL=$(echo "$HTML_DATA" | grep -o 'src="https://dailyverses\.net/images/[^"]*\.jpg' | head -n 1 | sed 's/src="//')
    fi

    if [[ -n "$IMG_URL" ]]; then
        ORIG_FILENAME=$(basename "$IMG_URL")
        TARGET_IMG="$WALLPAPER_DIR/$ORIG_FILENAME"
        
        # Download unique item
        curl -sL -A "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7)" -o "$TARGET_IMG" "$IMG_URL"
        
        # Override screens statically at login
        osascript -e "tell application \"System Events\" to tell every desktop to set picture to \"$TARGET_IMG\""
        
        # Initialize tracking states
        echo "$TODAY" > "$LAST_RUN_FILE"
        echo "0" > "$TIMER_FILE"
        echo "$TARGET_IMG" > "$CURRENT_IMG_FILE"
        echo "New verse displayed: $ORIG_FILENAME"
    else
        echo "Parsing error."
        exit 1
    fi
fi

# --- The 15-Minute Countdown Loop ---
while true; do
    TIME_SPENT=$(cat "$TIMER_FILE")
    if [[ "$TIME_SPENT" -ge 900 ]]; then
        restore_shuffle
        rm -f "$TIMER_FILE"
        rm -f "$CURRENT_IMG_FILE"
        break
    fi
    
    sleep 10
    echo "$((TIME_SPENT + 10))" > "$TIMER_FILE"
done