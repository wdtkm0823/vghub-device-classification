import networkx as nx
import numpy as np
from collections import deque
import time
import matplotlib.pyplot as plt
from networkx.algorithms.flow import shortest_augmenting_path

#行列の代入
filename = './vertexes4.csv' #内部VG-HubネットワークGの隣接行列A
filename_1 = './sink_vghub4_3.csv'
filename_2 = './source_vghub4_3.csv'
filename_3 = './addright_vghub4_3.csv'
filename_4 = './addbottom_vghub4_3.csv'


myarray = np.loadtxt(filename, delimiter=',') 
myarray_1 = np.loadtxt(filename_1, delimiter=',') 
myarray_2 = np.loadtxt(filename_2, delimiter=',')
myarray_3 = np.loadtxt(filename_3, delimiter=',')
myarray_4 = np.loadtxt(filename_4, delimiter=',')
    
time_sta = time.perf_counter()

tim_1=0
tim_2=0
tim_3=0
tim_4=0
tim_5=0
tim_6=0

for i in range(1):

    time_sta_1 = time.perf_counter()
    G = nx.from_numpy_array(myarray)
    N = nx.number_of_nodes(G)
    print(N)
    time_end_1 = time.perf_counter()
    tim_1 += time_end_1 - time_sta_1

    time_sta_2 = time.perf_counter()
    myarray_5 = np.concatenate((myarray,myarray_2),axis = 0) 
    myarray_6 = np.concatenate((myarray_5,myarray_1),axis = 1)
    time_end_2 = time.perf_counter()
    tim_2 += time_end_2 - time_sta_2
    
    time_sta_3 = time.perf_counter()
    myarray_7 = np.concatenate((myarray_6,myarray_4),axis = 0) 
    myarray_8 = np.concatenate((myarray_7,myarray_3),axis = 1)
    time_end_3 = time.perf_counter()
    tim_3 += time_end_3 - time_sta_3

    time_sta_4 = time.perf_counter()
    G = nx.from_numpy_array(myarray_8)
    time_end_4 = time.perf_counter()
    tim_4 += time_end_4 - time_sta_4

    nx.draw_networkx(G,with_labels=True)
    plt.show()

    time_sta_5 = time.perf_counter()
    flow_value, flows = nx.maximum_flow(G,N+6,N+7,capacity='weight')
    time_end_5 = time.perf_counter()
    tim_5 += time_end_5 - time_sta_5

time_end = time.perf_counter()

print(flow_value)

tim = time_end - time_sta
print('entirety:   %s' %tim)

print('number of N:%s' %tim_1)

print('concatenate:%s' %tim_2)

print('add:     %s' %tim_3)

print('conver:        %s' %tim_4)

tim_6=tim_2+tim_3
print('graph:      %s' %tim_6)

print('maxflow:    %s' %tim_5)
