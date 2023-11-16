#!/bin/bash

read -p "15s or pdnego : " file

# 移動元となるフォルダ
source_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/VirtualGridHub_product_classify/eval_csv/dataset_${file}_20231024/cwt"

read -p "test or validation or train : " data

read -p "cheeropowerplus5 or pixel3a or ipadair4th or xperiaxz2compact : " device

# 移動先のフォルダ
destination_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/VirtualGridHub_product_classify/eval_csv/dataset_${file}_20231024/${data}/${device}#1"

# パターンに一致するファイルを移動先のフォルダに移動させる
mv "$source_folder"/output_vghub_"$file"_data_3*.csv "$destination_folder"