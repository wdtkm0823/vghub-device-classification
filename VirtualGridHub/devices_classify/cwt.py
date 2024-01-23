import os
import numpy as np
import pandas as pd
import seaborn as sns
from pylab import rcParams
import matplotlib.pyplot as plt
from matplotlib import rc
from pandas.plotting import register_matplotlib_converters
import glob
from scipy import signal

method_options = {1: "15s", 2: "pdnego"}
dataset_options = {1: "test", 2: "validation", 3: "train"}
device_options = {
    1: "cheeropowerplus5",
    2: "cheeropowermountain",
    3: "omnichargeomni20+",
    4: "googlepixel3a",
    5: "ipadair4th",
    6: "xperiaxz2compact"
}

# Ask for the method
method_option = int(input("Which method is it, 1 for 15s or 2 for pdnego? : "))
method = method_options.get(method_option)
if method is None:
    print("Invalid option for method.")
    exit(1)

# Ask for the dataset
dataset_option = int(input("Which data set is it, 1 for test, 2 for validation, or 3 for train? : "))
dataset = dataset_options.get(dataset_option)
if dataset is None:
    print("Invalid option for dataset.")
    exit(1)

# Ask for the device
device_option = int(input("Which device is it, 1 for cheeropowerplus5, 2 for cheeropowermountain, 3 for omnichargeomni20+, 4 for googlepixel3a, 5 for ipadair4th, 6 for xperiaxz2compact? : "))
device = device_options.get(device_option)
if device is None:
    print("Invalid option for device.")
    exit(1)

ROOT_DIR = f'/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_{method}_20231024/cwt'
TARGET_PATTERN = "**.csv"
SAVEPATH = f'/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_cwt/dataset_{method}_20231024/{dataset}/{device}#1'

register_matplotlib_converters()
sns.set(style='whitegrid', palette='muted', font_scale=1.5)

rcParams['figure.figsize'] = 22, 10

#データセット分の画像を生成
def make_cwt_dataset(data_path, save_a_path):
    figure_size=(50,50)
    for j, d in enumerate(data_path):
        w = ImportCSVandConvertDF(d)
        cwt_arr=calcuate_cwt_ricker(w)
        length_path = len(str(d).split("/"))
        save_filename = str(d).split("/")[length_path-1].split(".csv")[0]+"_ricker"
        plot_cwt_save(cwt_arr,figure_size,save_a_path,save_filename)
    print('CWT DATASET DONE...')

# 一つのcsvファイルからVBUS V I Wを返す
def ImportCSVandConvertDF(FILE):
    data=pd.read_csv(FILE)
    data_V=data.iloc[:,1]
    data_C=data.iloc[:,2]
    data_W=data.iloc[:,3]
    npdata_V=data_V.values
    npdata_V
    npdata_C=data_C.values
    npdata_C
    npdata_W=data_W.values
    npdata_W
    return npdata_W

# rickerウェーブレット
def calcuate_cwt_ricker(sig):
    widths = np.arange(1, 31)
    cwtmatr = signal.cwt(sig, signal.ricker, widths)
    return cwtmatr

#CWTを画像で保存
def plot_cwt_save(cwtmatr_,figure_size,SAVEPATH,FILENAME):
    plt.imshow(cwtmatr_, extent=[-1, 1, 1, 31], cmap='gray', aspect='auto')
    plt.xlabel("Time[s]")
    plt.ylabel("Frequency[Hz]")
    plt.axis("off")
    plt.savefig(SAVEPATH+'/'+FILENAME+'.png')
    plt.close('all')

def cwt():
    root_a_path=[]
    root_a_path.extend(glob.glob(os.path.join(ROOT_DIR, TARGET_PATTERN)))
    root_a_test_path = root_a_path
    #csvからcwtの画像を生成し、データセットに保存
    make_cwt_dataset(root_a_test_path, SAVEPATH)
    filename = glob.glob(os.path.join(SAVEPATH, '*.png'))
cwt()

