#!/bin/bash

# The folder from which to move the file.
source_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/main/sampling_distribution/python/output"

# Ask for the method
read -p "1: 15s\n2: pdnego\nEnter the option number: " method_number

case $method_number in
    1)
        method="15s"
        ;;
    2)
        method="pdnego"
        ;;
    *)
        echo "Invalid option for method."
        exit 0
        ;;
esac

# Set the source folder based on the method
destination_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_${method}_20231024/cwt"

# Move files matching the pattern to the destination folder.
mv "$source_folder"/output_vghub_"$method"_data_3*.csv "$destination_folder"
