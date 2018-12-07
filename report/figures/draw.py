import matplotlib.pyplot as plt
import math

x = [2000000000,	1000000000,	500000000,	200000000,	100000000]
y1 = [5600433,	2313902,	1195260,	433214,	235116]
y2 = [5189967,	2155838,	1055295,	435434,	262033]

plt.plot(x, y1, label = 'default')
plt.plot(x, y2, label = 'mergeFunction')
plt.legend(loc = 'upper left')
plt.xlabel('iterations')
plt.ylabel('running time(us)')
plt.savefig('mergefunc.png')
