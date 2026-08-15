I cannot directly upload or export files to your personal Google Drive account because I do not have direct write access to your cloud storage.
However, since you have the "Export to Docs" option available at the bottom of our chat window, you can use that built-in shortcut to save this information straight to your Google Drive as a Google Doc, which you can then easily download as a Markdown (.md) file if you wish.
Here is the clean text layout for you to export or copy. Once you click Export to Docs, you can open that document in Google Docs, go to File > Download > Markdown (.md), and it will download perfectly to your Mac.
macOS Daily Wallpaper Script Backup
This configuration runs a native zsh script that fetches a daily verse wallpaper from dailyverses.net, saves it to an iCloud Drive folder, forces macOS to display it instantly on your first login of the day, and then hands control back to your regular wallpaper slideshow rotation.
1. The Core zsh Script
Save the following script content as ~/fetch_verse.sh on your Mac.



Bash
```
#!/bin/zsh

# Configurations
WALLPAPER_DIR="$HOME/Library/Mobile Documents/com~apple~CloudDocs/Pictures/Slideshow"
TARGET_IMG="$WALLPAPER_DIR/daily_verse.jpg"
TARGET_URL="https://dailyverses.net"
LAST_RUN_FILE="$HOME/.config/daily_verse_last_run"

# Ensure the config directory exists
mkdir -p "$HOME/.config"
mkdir -p "$WALLPAPER_DIR"

# Get today's date (format: YYYY-MM-DD)
TODAY=$(date "+%Y-%m-%d")

# Check if the script has already run successfully today
if [[ -f "$LAST_RUN_FILE" ]]; then
    LAST_RUN=$(cat "$LAST_RUN_FILE")
    if [[ "$LAST_RUN" == "$TODAY" ]]; then
        echo "Script already executed today ($TODAY). Exiting to preserve slideshow."
        exit 0
    fi
fi

# Fetch HTML once
HTML_DATA=$(curl -sL -A "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7)" "$TARGET_URL")

# Isolate the specific verse-named image path (OpenGraph Meta)
IMG_URL=$(echo "$HTML_DATA" | grep -o 'meta property="og:image" content="[^"]*' | awk -F'content="' '{print $2}')

# Fallback parse step
if [[ -z "$IMG_URL" ]]; then
    IMG_URL=$(echo "$HTML_DATA" | grep -o 'src="https://dailyverses\\.net/images/[^"]*\\.jpg' | head -n 1 | sed 's/src="//')
fi

# Action the file retrieval
if [[ -n "$IMG_URL" ]]; then
    echo "Identified Daily Verse Image Path: $IMG_URL"
    curl -sL -A "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7)" -o "$TARGET_IMG" "$IMG_URL"
    
    # Force macOS to instantly render the freshly downloaded image
    osascript -e "tell application \"System Events\" to tell every desktop to set picture to \"$TARGET_IMG\""
    
    # Save today's date so it won't run again until tomorrow
    echo "$TODAY" > "$LAST_RUN_FILE"
    echo "Wallpaper updated successfully for $TODAY."
else
    echo "Parsing error: Unable to detect any verse-named image layout."
fi
```
Make It Executable
After saving the file, open your Terminal and apply execution permissions:



Bash
```
chmod +x ~/fetch_verse.sh
```

2. The Launch Agent Property List (.plist)
To make macOS automatically trigger this check every single time you log into your desktop user profile, create a background Launch Agent plist file at ~/Library/LaunchAgents/com.user.dailyverse.plist.


XML
```

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.EN.dtd">
<plist version="1.0">
<dict>
    <key>Label</key>
    <string>com.user.dailyverse</string>
    <key>ProgramArguments</key>
    <array>
        <string>/bin/zsh</string>
        <string>-c</string>
        <string>~/fetch_verse.sh</string>
    </array>
    <key>RunAtLoad</key>
    <true/>
</dict>
</plist>
```
Loading the Agent
To immediately activate the daemon without needing to log out and log back in, register the job with macOS by running:



Bash
```
launchctl load ~/Library/LaunchAgents/com.user.dailyverse.plist
```
3. Execution Logic
1.	Initial Morning Login: When you input your password for the first time during a new calendar date, launchd triggers the background task. The script verifies that your hidden file at ~/.config/daily_verse_last_run does not contain today's string representation (YYYY-MM-DD). It handles the HTTP download stream via curl, overwrites the previous daily_verse.jpg in your iCloud Slideshow target directory, and then enforces an explicit application framework call (osascript) to flash the image across all active monitors. Finally, it stamps today's date to lock execution.
2.	Mid-day System Restart / Re-login: If you power cycle your Mac, log out to swap profiles, or lose power later in the day, the plist executes the script again upon return. However, the script evaluates that the target run file matches today's exact date stamp. It immediately exits with code 0 in a matter of milliseconds. This prevents your active native macOS wallpaper slideshow interval from being continuously broken or overwritten. Your normal rotation pool operates entirely unhindered.
