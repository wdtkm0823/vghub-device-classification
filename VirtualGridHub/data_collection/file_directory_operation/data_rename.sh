#!/bin/sh

# 対象ディレクトリのパスを指定します
directory="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_15s_20231024/cwt/"

# ディレクトリ内のファイルを順に処理します
for file in "$directory"*soc20*; do
    new_filename=$(echo "$file" | sed 's/soc20/soc30/')
    mv "$file" "$new_filename"
    echo "ファイル名を変更しました: $file を $new_filename に変更しました。"
done
