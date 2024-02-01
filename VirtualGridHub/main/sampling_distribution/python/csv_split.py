import sys
import pandas as pd
import numpy as np
import os

def make_path(output_name, index, soc, data, app, device):
    port_num = 7
    output_path = "python/output/"
    output_paths = []
    for i in range(port_num):
        if i == 3:
            output_paths.append(os.path.join(output_path,output_name+"_"+str(i)+"_"+device+"_"+"soc"+str(soc)+"_"+app+"_"+data+"_"+index+".csv"))
        else:
            output_paths.append(os.path.join(output_path,output_name+"_"+str(i)+".csv"))
    return output_paths

def convert_csv(csv, output_paths):
    i=2
    port=0
    while (i<16):
        time_name = csv.iloc[:, [0]].columns[0]
        slope_name = csv.iloc[:, [16]].columns[0]
        mampare_name = csv.iloc[:, [i]].columns[0]
        mvoltage_name = csv.iloc[:, [i+1]].columns[0]
        wattage = round(csv[mvoltage_name]/1000*csv[mampare_name]/1000,3)
        df_wattage = pd.DataFrame({'Wattage[W]': wattage})
        df_port = pd.concat([round(csv[time_name], 3), round(csv[slope_name],3), csv[mvoltage_name]/1000, csv[mampare_name]/1000, df_wattage], axis=1)
        df_port = df_port.rename(
            columns={time_name: 'Timestamp[us]',slope_name: 'Slope', mvoltage_name: 'VBUS Voltage[V]', mampare_name: 'VBUS Current[A]', 'Wattage[W]': 'VBUS Wattage[W]'})
        df_port.to_csv(output_paths[port], index=False)
        i = i+2
        port+=1

def input_csv(csv_input_path):
    return pd.read_csv(csv_input_path)

def main():
    os.getcwd
    args = sys.argv
    print()
    print("File name: ",args[1])
    print("Number of data: ",args[2])
    print("SOC: ", args[3])
    print("Data: ",args[4])
    print("Application: ",args[5])
    print("Device: ",args[6])
    output_name = "output_" + args[1]
    output_paths = make_path(output_name, args[2], args[3], args[4], args[5], args[6])
    csv_input_path = os.path.join("python","csv", args[1]+".csv")
    csv = input_csv(csv_input_path)
    convert_csv(csv, output_paths)
    print(output_name + ".csv split DONE")

main()
