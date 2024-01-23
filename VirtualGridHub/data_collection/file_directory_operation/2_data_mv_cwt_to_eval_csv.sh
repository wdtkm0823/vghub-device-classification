#!/bin/bash

# Ask for the method
echo -e "1: 15s\n2: pdnego"
read -p "Enter the option number: " method_number

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
echo -e "1: test\n2: train\n3: validation"
read -p "Enter the option number: " dataset_number

case $dataset_number in
    1)
        dataset="test"
        ;;
    2)
        dataset="train"
        ;;
    3)
        dataset="validation"
        ;;
    *)
        echo "Invalid option for dataset."
        exit 0
        ;;
esac

# Ask for the device
echo -e "1: cheeropowermountain\n2: cheeropowerplus5\n3: googlepixel3a\n4: ipadair4th\n5: omnichargeomni20+\n6: xperiaxz2compact"
read -p "Enter the option number: " device_number

case $device_number in
    1)
        device="cheeropowermountain"
        ;;
    2)
        device="cheeropowerplus5"
        ;;
    3)
        device="googleppixel3a"
        ;;
    4)
        device="ipadair4th"
        ;;
    5)
        device="omnichargeomni20+"
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
