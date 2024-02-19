#!/bin/bash

# Ask for the method
echo "1: 15s\n2: PDNego"
read -p "Enter the option number: " method_number

case $method_number in
    1)
        method_folder="15s"
        method_file="15s"
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
source_folder="/home/pi/Desktop/Doyer/Research/VirtualGridHub/DataCollection/EvalCSV/Dataset${method_folder}/CWT"

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
echo "1: AnkerPowerCore3#1\n2: AnkerPowerCore3#2\n3: cheeroPowerMountain#1\n4: cheeroPowerPlus5#1\n5: GooglePixel3a#1\n6: iPadAir4th#1\n7: OmnichargeOmni20+#1\n8: XperiaXZ2Compact#1"
read -p "Enter the option number: " device_number

case $device_number in
    1)
        device="AnkerPowerCore3#1"
        ;;
    2)
        device="AnkerPowerCore3#2"
        ;;
    3)
        device="cheeroPowerMountain#1"
        ;;
    4)
        device="cheeroPowerPlus5#1"
        ;;
    5)
        device="GooglePixel3a#1"
        ;;
    6)
        device="iPadAir4th#1"
        ;;
    7)
        device="OmnichargeOmni20+#1"
        ;;
    8)
        device="XperiaXZ2Compact#1"
        ;;
    *)
        echo "Invalid option for device."
        exit 0
        ;;
esac

# Set the destination folder based on method, dataset, and device
destination_folder="/home/pi/Desktop/Doyer/Research/VirtualGridHub/DataCollection/EvalCSV/Dataset${method_folder}/${dataset}/${device}"

# Move files matching the pattern to the destination folder.
mv "$source_folder"/output_vghub_"$method_file"_data_3*.csv "$destination_folder"
