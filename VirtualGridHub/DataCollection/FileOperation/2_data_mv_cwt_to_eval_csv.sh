#!/bin/bash

# Ask for the method
echo "1: 15s\n2: PDNego"
read -p "Enter the option number: " method_number

case $method_number in
    1)
        method="15s"
        ;;
    2)
        method="PDNego"
        ;;
    *)
        echo "Invalid option for method."
        exit 0
        ;;
esac

# Set the source folder based on the method
source_folder="/home/pi/Desktop/Doyer/Research/VirtualGridHub/DevicesClassify/EvalCSV/Dataset${method}/CWT"

# Ask for the dataset
echo "1: Test\n2: Train\n3: Validation"
read -p "Enter the option number: " dataset_number

case $dataset_number in
    1)
        dataset="Test"
        ;;
    2)
        dataset="Train"
        ;;
    3)
        dataset="Validation"
        ;;
    *)
        echo "Invalid option for dataset."
        exit 0
        ;;
esac

# Ask for the device
echo "1: AnkerPowerCore3\n2: cheeroPowerMountain\n3: cheeroPowerPlus5\n4: Pixel3a\n5: iPadAir4th\n6: OmnichargeOmni20+\n7: XperiaXZ2Compact\nEnter the option number: "
read -p "Enter the option number: " device_number

case $device_number in
    1)
        device="AnkerPowerCore3"
        ;;
    2)
        device="cheeroPowerMountain"
        ;;
    3)
        device="cheeroPowerPlus5"
        ;;
    4)
        device="GooglePixel3a"
        ;;
    5)
        device="iPadAir4th"
        ;;
    6)
        device="OmnichargeOmni20+"
        ;;
    7)
        device="XperiaXZ2Compact"
        ;;
    *)
        echo "Invalid option for device."
        exit 0
        ;;
esac

# Set the destination folder based on method, dataset, and device
destination_folder="/home/pi/Desktop/Doyer/Research/VirtualGridHub/DevicesClassify/EvalCSV/Dataset${method}/${dataset}/${device}#1"

# Move files matching the pattern to the destination folder.
mv "$source_folder"/output_vghub_"$method"_data_3*.csv "$destination_folder"
