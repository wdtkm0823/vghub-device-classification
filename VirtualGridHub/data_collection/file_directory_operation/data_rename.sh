#!/bin/sh

# 対象ディレクトリのパスを指定します
directory="/home/pi/Desktop/doyer/research/VirtualGridHub/main/sampling_distribution/python/output/"

# ディレクトリ内のファイルを順に処理します
for file in "$directory"*validation*; do
    new_filename=$(echo "$file" | sed 's/validation/test/')
    mv "$file" "$new_filename"
    echo "ファイル名を変更しました: $file を $new_filename に変更しました。"
done
