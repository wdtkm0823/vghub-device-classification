#!/bin/bash

# The folder from which to move the file.
source_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/main/sampling_distribution/python/output"

read -p "15s or pdnego : " file

# The folder to which to move the file.
destination_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_${file}_20231024/cwt"

# Move files matching the pattern to the destination folder.
mv "$source_folder"/output_vghub_"$file"_data_3*.csv "$destination_folder"
