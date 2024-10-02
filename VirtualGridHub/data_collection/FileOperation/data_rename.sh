#!/bin/sh

# Specify the path of the target directory
directory="/home/pi/Desktop/doyer/research/VirtualGridHub/main/sampling_distribution/python/output/"

# Processes files in directories in sequence
for file in "$directory"*omni20+*; do
    new_filename=$(echo "$file" | sed 's/omni20+/omnichargeomni20+/')
    mv "$file" "$new_filename"
    echo "File name has been changed.: Changed $file to $new_filename."
done
