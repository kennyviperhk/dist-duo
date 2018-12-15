#!/bin/bash
#/home/pi/of_v0.10.1_linuxarmv6l_release/apps/dist-duo/Release-Dist-Duo/bin/Release-Dist-Duo
while [ true ]; do
  until /home/pi/of_v0.10.1_linuxarmv6l_release/apps/dist-duo/Release-Dist-Duo/bin/Release-Dist-Duo ; do
      echo "Restart App $?.  Respawning.." >&2
      sleep 1
  done
done
