#!/bin/bash
#/home/pi/openFrameworks/apps/myApps/Undermine/bin/Undermine
while [ true ]; do
  until /home/pi/openFrameworks/apps/myApps/Undermine/bin/Undermine ; do
      echo "Restart App $?.  Respawning.." >&2
      sleep 1
  done
done
