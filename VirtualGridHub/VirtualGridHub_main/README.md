# VGHub_MA
this repository is wada's developing repository of vghub .


# how to gompile
gcc -o Myapp Myapp.c libVGHubPD.a -lpthread -lwiringPi -lrt



# directory

/C_Call_python
CからPythonを呼ぶためコード

/code
頂いたコードやM1のときのコード，ほとんど触らない．

/mytraining
適当なTestフォルダ

/sampling
サンプリングができるようにしたもの．
python
numpy
pandasが必要．
sorce activate py36

/sampling_distribusion
サンプリング，電力分配ができるようにしたもの．
python
numpy
pandasが必要．
sorce activate py36

Sampling_Dist_V3_20211024.c　のport名と電力割付を記入しコンパイルしたらサンプリングされる
VGHub_MA/sampling_distribution/python/csv　内部には，VGHUBの生データが保管される
VGHub_MA/sampling_distribution/python/output 内部には，わっとみると同じファイル形式で保管される．ファイルはそれぞれportごとに分割されている

/sampling_distribution_googlepost
サンプリング，電力分配ができる．サンプリングデータをgoogleDriveにおけるようにした（ほぼつかわない）
python
numpy
pandasが必要．
sorce activate py36


/sampling_distribution_onvghubPython
サンプリング，電力分配ができるようにしたもの．（ほぼつかわない）
python
numpy
pandasが必要．
sorce activate py36



# 構成
/sampling_distribusion_googlepost
-サンプリング＋googlepost
-colabで推論
-定期的にgooglegetする関数
-flaskで表示する関数