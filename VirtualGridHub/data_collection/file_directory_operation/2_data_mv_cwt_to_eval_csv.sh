#!/bin/bash

read -p "which method it it, 15s or pdnego? : " method

# The folder from which to move the file.
source_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_${method}_20231024/cwt"

read -p "Which data set is it, test or validation or train? : " dataset

read -p "Which device is it, cheeropowerplus5 or cheeropowermountain or omnichargeomni20+ or googlepixel3a or ipadair4th or xperiaxz2compact? : " device

# The folder to which to move the file.
destination_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_${method}_20231024/${dataset}/${device}#1"

# Move files matching the pattern to the destination folder.
mv "$source_folder"/output_vghub_"$method"_data_3*.csv "$destination_folder"