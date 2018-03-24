#!/bin/bash
sudo cat /../dev/ttyACM0 | while read var1; do
echo $var1
pico2wave -w morse.wav "$var1" && aplay morse.wav
done