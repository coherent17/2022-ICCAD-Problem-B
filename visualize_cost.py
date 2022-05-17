import matplotlib.pyplot as plt
import numpy as np


file = open("cost.out", 'r')
cost = []
for line in file:
    cost.append(line)
    print(line)

for i in range(0,len(cost)):
    plt.plot(i,cost[i],'b.')
plt.ylim(min(cost),max(cost))

plt.savefig('cost.svg')