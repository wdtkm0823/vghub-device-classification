#!/bin/sh

# 対象ディレクトリのパスを指定します
directory="/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_cwt/dataset_pdnego_20231024/train/cheeropowerplus5#1/"

# ディレクトリ内のファイルを順に処理します
for file in "$directory"*xperiaxz2compact#1*; do
    # ファイル名から"xperiaxz2compact#1"を"cheeropowerplus5#1"に置換して変更します
    new_filename=$(echo "$file" | sed 's/xperiaxz2compact#1/cheeropowerplus5#1/')
    mv "$file" "$new_filename"
    echo "ファイル名を変更しました: $file を $new_filename に変更しました。"
done
