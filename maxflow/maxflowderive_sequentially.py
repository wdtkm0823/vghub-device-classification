import networkx as nx
import numpy as np
from collections import deque
import time
import matplotlib.pyplot as plt
from networkx.algorithms.flow import shortest_augmenting_path

filename = './vertexes10.csv'
myarray = np.loadtxt(filename, delimiter=',') 

time_sta = time.perf_counter()

tim_1=0
tim_2=0
tim_4=0

for i in range(1000):

    time_sta_1 = time.perf_counter()
    G = nx.from_numpy_matrix(myarray)
    N=nx.number_of_nodes(G)
    time_end_1 = time.perf_counter()
    tim_1 += time_end_1 - time_sta_1

    time_sta_2 = time.perf_counter()
    G.add_edge(N,0)
    G.add_edge(N,0)
    G.add_edge(N,0)
    G.add_edge(N,1)
    G.add_edge(N,1)
    G.add_edge(N,1)
    G.add_edge(N,2)
    G.add_edge(N,2)
    G.add_edge(N,2)
    G.add_edge(N,3)
    G.add_edge(N,3)
    G.add_edge(N,3)
    G.add_edge(N,4)
    G.add_edge(N,4)
    G.add_edge(N,4)
    G.add_edge(5,N+1)
    G.add_edge(5,N+1)
    G.add_edge(5,N+1)
    G.add_edge(6,N+1)
    G.add_edge(6,N+1)
    G.add_edge(6,N+1)
    G.add_edge(7,N+1)
    G.add_edge(7,N+1)
    G.add_edge(7,N+1)
    G.add_edge(8,N+1)
    G.add_edge(8,N+1)
    G.add_edge(8,N+1)
    G.add_edge(9,N+1)
    G.add_edge(9,N+1)
    G.add_edge(9,N+1)
    time_end_2 = time.perf_counter()
    tim_2 += time_end_2 - time_sta_2

    #nx.draw_networkx(G,with_labels=True)
    #plt.show()
    #R = shortest_augmenting_path(G,N,N+1,capacity='weight')
    time_sta_4 = time.perf_counter()
    flow_value, flows = nx.maximum_flow(G,N,N+1,capacity='weight')
    time_end_4 = time.perf_counter()
    tim_4 += time_end_4 - time_sta_4

time_end = time.perf_counter()
print(flow_value)

tim = time_end - time_sta
print('entirety:   %s' %tim)

print('number of N:%s' %tim_1)

print('graph:      %s' %tim_2)

print('maxflow:   %s' %tim_4)
