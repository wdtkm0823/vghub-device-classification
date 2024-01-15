#!/bin/sh

# Specify the path of the target directory
directory="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_pdnego_20231024/cwt/"

# Processes files in directories in sequence
for file in "$directory"*test*; do
    new_filename=$(echo "$file" | sed 's/test/validation/')
    mv "$file" "$new_filename"
    echo "File name has been changed.: Changed $file to $new_filename."
done
