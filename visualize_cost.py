import matplotlib.pyplot as plt
import numpy as np


file = open("cost.out", 'r')
cost = file.readline().split(',')

cost_max = -1

#plt.figure(figsize=(20, 6))
for i in range(0,len(cost)-1):
    if(int(cost[i]) > cost_max):
        cost_max = int(cost[i])
    plt.plot(i,int(cost[i]),"ob:")

plt.title('cost when accept the M1 change')
plt.ylim(0,cost_max*1.5)

plt.savefig('cost.svg')