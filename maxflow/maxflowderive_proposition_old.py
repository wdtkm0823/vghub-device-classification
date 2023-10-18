import networkx as nx
import numpy as np
from collections import deque
import time
import matplotlib.pyplot as plt
from networkx.algorithms.flow import shortest_augmenting_path

filename = './vertexes4.csv'
filename_1 = './sink_vghub4_3.csv'
filename_2 = './source_vghub4_3.csv'


myarray = np.loadtxt(filename, delimiter=',') 
myarray_1 = np.loadtxt(filename_1, delimiter=',') 
myarray_2 = np.loadtxt(filename_2, delimiter=',')
    
time_sta = time.perf_counter()

tim_1=0
tim_2=0
tim_3=0
tim_4=0
tim_5=0
tim_6=0

for i in range(1000):

    time_sta_1 = time.perf_counter()
    G = nx.from_numpy_matrix(myarray)
    N = nx.number_of_nodes(G)
    time_end_1 = time.perf_counter()
    tim_1 += time_end_1 - time_sta_1

    time_sta_2 = time.perf_counter()
    myarray_3 = np.concatenate((myarray,myarray_2),axis = 0) 
    myarray_4 = np.concatenate((myarray_3,myarray_1),axis = 1)
    time_end_2 = time.perf_counter()
    tim_2 += time_end_2 - time_sta_2

    time_sta_3 = time.perf_counter()
    G = nx.from_numpy_matrix(myarray_4)
    time_end_3 = time.perf_counter()
    tim_3 += time_end_3 - time_sta_3
 
    time_sta_4 = time.perf_counter()
    G.add_edge(N+6,N,capacity=3.0)
    G.add_edge(N+6,N+1,capacity=3.0)
    G.add_edge(N+6,N+2,capacity=3.0)
    G.add_edge(N+3,N+7,capacity=3.0)
    G.add_edge(N+4,N+7,capacity=3.0)
    G.add_edge(N+5,N+7,capacity=3.0)
    time_end_4 = time.perf_counter()
    tim_4 += time_end_4 - time_sta_4

    #nx.draw_networkx(G,with_labels=True)
    #.show()

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

print('conver:     %s' %tim_3)

print('add:        %s' %tim_4)

tim_6=tim_2+tim_3+tim_4
print('graph:      %s' %tim_6)

print('maxflow:    %s' %tim_5)
