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

method_options = {1: "15s", 2: "PDNego"}
dataset_options = {1: "Test", 2: "Train", 3: "Validation"}
device_options = {
    1: "AnkerPowerCore3#1",
    2: "AnkerPowerCore3#2",
    3: "cheeroPowerMountain#1",
    4: "cheeroPowerPlus5#1",
    5: "GooglePixel3a#1",
    6: "iPadAir4th#1",
    7: "OmnichargeOmni20+#1",
    8: "JFPEACE8#1",
    9: "XperiaXZ2Compact#1",
}

# Ask for the method
method_option = int(input("1: 15s\n2: PDNego\nEnter the option number: "))
method = method_options.get(method_option)
if method is None:
    print("Invalid option for method.")
    exit(1)

# Ask for the dataset
dataset_option = int(input("1: Test\n2: Train\n3: Validation\nEnter the option number: "))
dataset = dataset_options.get(dataset_option)
if dataset is None:
    print("Invalid option for dataset.")
    exit(1)

# Ask for the device
device_option = int(input("1: AnkerPowerCore3#1\n2: AnkerPowerCore3#2\n3: cheeroPowerMountain#1\n4: cheeroPowerPlus5#1\n5: GooglePixel3a#1\n6: iPadAir4th#1\n7: OmnichargeOmni20+#1\n8: JFPEACE8#1\n9: XperiaXZ2Compact#1\nEnter the option number: "))
device = device_options.get(device_option)
if device is None:
    print("Invalid option for device.")
    exit(1)

ROOT_DIR = f'/home/pi/Desktop/Doyer/Research/VirtualGridHub/DataCollection/EvalCSV/Dataset{method}/CWT'
TARGET_PATTERN = "**.csv"
SAVEPATH = f'/home/pi/Desktop/Doyer/Research/VirtualGridHub/DataCollection/EvalCWT/Dataset{method}/{dataset}/{device}'

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

