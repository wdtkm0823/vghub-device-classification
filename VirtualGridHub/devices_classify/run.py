# Import necessary libraries
import os
import glob
import numpy as np
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
from pylab import rcParams
from matplotlib import rc
from pandas.plotting import register_matplotlib_converters
from sklearn.model_selection import train_test_split
from scipy import signal

# TensorFlow and Keras imports
import tensorflow as tf
from tensorflow.keras.preprocessing.image import load_img, img_to_array
from tensorflow.keras.models import Sequential, Model
from tensorflow.keras.layers import Input, Dense, Dropout, Flatten, GlobalAveragePooling2D
from tensorflow.keras.applications.vgg16 import VGG16, preprocess_input

# 以下の２行はJupyter環境の場合のみ必要
#%matplotlib inline
#from IPython import display
#%config InlineBackend.figure_format='retina'

register_matplotlib_converters()
sns.set(style='whitegrid', palette='muted', font_scale=1.5)

rcParams['figure.figsize'] = 22, 10

RANDOM_SEED = 42

np.random.seed(RANDOM_SEED)
tf.random.set_seed(RANDOM_SEED)

def my_model_load():
    base_model = VGG16(include_top=False, weights='imagenet', input_tensor = Input(shape=(224, 224, 3)))
    x = base_model.output
    x = GlobalAveragePooling2D()(x)
    x = Dense(1024, activation='relu')(x)
    prediction = Dense(nb_classes, activation='softmax')(x)
    model = Model(inputs=base_model.input, outputs=prediction)
    
    # input_tensor = Input(shape=(img_rows, img_cols, 3))
    # vgg16 = VGG16(include_top=False, weights=None, input_tensor=input_tensor)
    # top_model = Sequential()
    # top_model.add(Flatten(input_shape=vgg16.output_shape[1:]))
    # top_model.add(Dense(256, activation='relu'))
    # top_model.add(Dense(1024, activation='relu'))
    # top_model.add(Dropout(0.5))
    # top_model.add(Dense(nb_classes, activation='softmax'))
    # model = Model(inputs=vgg16.input, outputs=top_model(vgg16.output))
    model.summary()
    model.load_weights(os.path.join('model', 'devices_classify_cwt_cnn_pdnego_ep50_batch32.hdf5'))
    return model

# Return VBUS V I W from a single csv file
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

# ricker wavelet
def calcuate_cwt_ricker(sig):
    widths = np.arange(1, 31)
    cwtmatr = signal.cwt(sig, signal.ricker, widths)
    return cwtmatr

def make_cwt_dataset(data_path, save_a_path):
    figure_size=(50,50)
    for j, d in enumerate(data_path):
        w = ImportCSVandConvertDF(d)
        cwt_arr=calcuate_cwt_ricker(w)
        # mac
        # length_path = len(str(d).split("/"))
        # save_filename = str(d).split("/")[length_path-1].split(".csv")[0]+"_ricker"
        # windows
        length_path = len(str(d).split("\\"))
        save_filename = str(d).split("\\")[length_path-1].split(".csv")[0]+"_ricker"
        plot_cwt_save(cwt_arr,figure_size,save_a_path,save_filename)
    print("CWT DATASET COMPLETED")

def plot_cwt_save(cwtmatr_,figure_size,SAVEPATH,FILENAME):
    plt.imshow(cwtmatr_, extent=[-1, 1, 1, 31], cmap='gray', aspect='auto')
    plt.xlabel("Time[s]")
    plt.ylabel("Frequency[Hz]")
    plt.axis("off")
    plt.savefig(SAVEPATH+'/'+FILENAME+'.png')
    plt.close('all')

ROOT_DIR = 'eval_csv'
TARGET_PATTERN = "**.csv"
SAVEPATH = 'eval_cwt'
classes = ['cheero Power Mountain', 'cheero Power Plus 5', 'Google Pixel 3a', 'iPad Air 4th', 'Xperia XZ2 Compact']
nb_classes = len(classes)
img_rows, img_cols = 224, 224

def predict():
    root_a_path = []
    root_a_path.extend(glob.glob(os.path.join(ROOT_DIR, TARGET_PATTERN)))
    root_a_test_path = root_a_path
    # Generate cwt images from csv and store them in the dataset.
    make_cwt_dataset(root_a_test_path, SAVEPATH)
    filename = glob.glob(os.path.join(SAVEPATH, '*.png'))
    try:
        model = my_model_load()
        print("MODEL LOADING COMPLETED")
    except Exception as e:
        print(f"Model LOADING ERROR: {e}")
        return
    for target in filename:
        img = load_img(target, target_size=(img_rows, img_cols))
        x = img_to_array(img)
        x = np.expand_dims(x, axis=0)
        predict = model.predict(preprocess_input(x))
        for pre in predict:
            y = pre.argmax()
            class_name = classes[y]
            confidence = pre[y] * 100
            print("--------------------------------------------------")
            print(f"File Name: {target}")
            print("--------------------------------------------------")
            print(f"Prediction Class: {class_name}")
            print(f"Credibility: {confidence:.2f}%")
            print("--------------------------------------------------")
            print("Probability of each class:")
            for i, prob in enumerate(pre):
                print(f"({i+1}){classes[i]}: {prob * 100:.2f}%")
predict()
