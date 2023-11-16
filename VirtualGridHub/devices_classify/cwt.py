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

print("15s or pdnego : ")
file = input()

print("test or validation or train : ")
data = input()

print("cheeropowerplus5 or pixel3a or ipadair4th or xperiaxz2compact : ")
device = input()

ROOT_DIR = f'/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_csv/dataset_{file}_20231024/cwt'
TARGET_PATTERN = "**.csv"
SAVEPATH = f'/home/pi/Desktop/doyer/research/VirtualGridHub/devices_classify/eval_cwt/dataset_{file}_20231024/{data}/{device}#1'

classes = ['Pixel_#1','Pixel_#2','Pixel_#3']
nb_classes = len(classes)

img_rows, img_cols = 224, 224

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
    
# 一つのcsvファイルから VBUS V I W  を返す
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
    
# ricker ウェーブレット
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
    #csv から　cwt の画像を生成し，データセットに保存
    make_cwt_dataset(root_a_test_path, SAVEPATH)
    filename = glob.glob(os.path.join(SAVEPATH, '*.png'))
    
cwt()

