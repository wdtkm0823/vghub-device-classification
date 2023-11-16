#!/bin/sh

read -p "ファイル名：" file
gcc -o $file $file.c libVGHubPD.a -lpthread -lwiringPi -lrt -lm

exit 0
