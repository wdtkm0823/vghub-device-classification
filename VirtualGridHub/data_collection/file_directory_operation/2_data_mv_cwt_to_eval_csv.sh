#!/bin/bash

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
source_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_${method}_20231024/cwt"

# Ask for the dataset
read -p "1: test\n2: train\n3: validation\nEnter the option number: " dataset_number

case $dataset_number in
    1)
        dataset="test"
        ;;
    2)
        dataset="validation"
        ;;
    3)
        dataset="train"
        ;;
    *)
        echo "Invalid option for dataset."
        exit 0
        ;;
esac

# Ask for the device
read -p "Which device is it, 1 for cheeropowerplus5, 2 for cheeropowermountain, 3 for omnichargeomni20+, 4 for googlepixel3a, 5 for ipadair4th, 6 for xperiaxz2compact? : " device_number

case $device_number in
    1)
        device="cheeropowerplus5"
        ;;
    2)
        device="cheeropowermountain"
        ;;
    3)
        device="omnichargeomni20+"
        ;;
    4)
        device="googlepixel3a"
        ;;
    5)
        device="ipadair4th"
        ;;
    6)
        device="xperiaxz2compact"
        ;;
    *)
        echo "Invalid option for device."
        exit 0
        ;;
esac

# Set the destination folder based on method, dataset, and device
destination_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_${method}_20231024/${dataset}/${device}#1"

# Move files matching the pattern to the destination folder.
mv "$source_folder"/output_vghub_"$method"_data_3*.csv "$destination_folder"
