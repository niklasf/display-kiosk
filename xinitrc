#!/bin/sh

# Disable screen blanking.
xset s off
xset s noblank
xset -dpms

# Start display kiosk.
exec /usr/bin/display-kiosk
