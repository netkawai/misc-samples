#!/bin/sh
export XDG_MENU_PREFIX=gnome-
#xrandr --output DVI-I-0 --off --output DVI-I-1 --primary --mode 1920x1080 --pos 0x0 --rotate normal --output DP-0 --mode 1920x1080 --pos 1920x0 --rotate normal --output DP-1 --off --output DP-2 --off --output DP-3 --off
export XDG_SESSION_TYPE=x11
export XDG_BACKEND=x11
gnome-session
