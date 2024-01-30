#!/bin/bash

# The folder from which to move the file.
source_folder="/home/pi/Desktop/Doyer/Research/VirtualGridHub/main/sampling_distribution/python/output"

# Ask for the method
echo "1: 15s\n2: PDNego"
read -p "Enter the option number: " method_number

case $method_number in
    1)
        method="15s"
        ;;
    2)
        method_folder="PDNego"
        method_file="pdnego"
        ;;
    *)
        echo "Invalid option for method."
        exit 0
        ;;
esac

# Set the source folder based on the method
destination_folder="/home/pi/Desktop/Doyer/Research/VirtualGridHub/DevicesClassify/EvalCSV/Dataset${method_folder}/CWT"

# Move files matching the pattern to the destination folder.
mv "$source_folder"/output_vghub_"$method_file"_data_3*.csv "$destination_folder"
