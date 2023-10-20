import sys
import pandas as pd
import numpy as np
import os


def make_path(output_name):
    port_num = 7
    
    output_path = "python/output/"
    output_paths = []
    for i in range(port_num):
        #print(output_path+output_name+"_"+str(i)+".csv")
        output_paths.append(os.path.join(output_path,output_name+"_"+str(i)+".csv"))
    return output_paths

def convert_csv(csv, output_paths):
    i=2
    port=0
    while (i<16):
        #print(csv.iloc[:, [0, 1, i, i+1]])
        time_name = csv.iloc[:, [0]].columns[0]
        count_name = csv.iloc[:, [1]].columns[0]
        mampare_name = csv.iloc[:, [i]].columns[0]
        mvoltage_name = csv.iloc[:, [i+1]].columns[0]
        wattage = round(csv[mvoltage_name]/1000*csv[mampare_name]/1000,3)
        
        df_wattage = pd.DataFrame({'Wattage[W]': wattage})
        df_port = pd.concat(
            [round(csv[time_name],3), round(csv[count_name],3), csv[mvoltage_name]/1000, csv[mampare_name]/1000, df_wattage], axis=1)
        
        df_port=df_port.rename(
            columns={mvoltage_name: 'Voltage[V]', mampare_name: 'Ampare[A]'})
        #print(df_port)
        #print(port)    
        df_port.to_csv(output_paths[port], index=False)
        i = i+2
        port+=1

def input_csv(csv_input_path):
    return pd.read_csv(csv_input_path)

def main():
    print(os.getcwd())
    args = sys.argv
    # args[1] #input csv 名 
    print(args[1])
    
    output_name = "output_" + args[1]
    output_paths = make_path(output_name)
    csv_input_path = os.path.join("python","csv", args[1]+".csv")
    csv = input_csv(csv_input_path)
    convert_csv(csv, output_paths)
    print("csv split DONE")

main()
