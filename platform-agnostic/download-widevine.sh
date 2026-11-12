#!/bin/bash
LATEST=`curl https://dl.google.com/widevine-cdm/current.txt`
wget https://dl.google.com/widevine-cdm/$LATEST-linux-x64.zip
wget https://dl.google.com/widevine-cdm/$LATEST-win-x64.zip
wget https://dl.google.com/widevine-cdm/$LATEST-mac-x64.zip

