#!/bin/bash

# 移動元となるフォルダ
source_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/VirtualGridHub_main/sampling_distribution/python/output"

read -p "15s or pdnego : " file

# 移動先のフォルダ
destination_folder="/home/pi/Desktop/doyer/research/VirtualGridHub/VirtualGridHub_product_classify/eval_csv/dataset_${file}_20231024/cwt"

# パターンに一致するファイルを移動先のフォルダに移動させる
mv "$source_folder"/output_vghub_"$file"_data_3*.csv "$destination_folder"
