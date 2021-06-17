import matplotlib.pyplot as plt
import numpy as np
import os
import sys

fname = sys.argv[1] if len(sys.argv) >= 2 else input('Enter profiler file name: ')
offset = int(sys.argv[2]) if len(sys.argv) >= 3 else 0

with open(fname, 'r') as f:
    lines = f.readlines()
    data = [line.split(',') for line in lines]

    maxInstances = np.amax([len(entry) - 3 - offset for entry in data])


for entry in data:
    print(entry[0])
    print('Mean:', entry[1], 'ms')
    print('Std dev:', entry[2], 'ms\n')

    values = [float(val) for val in entry[3 + offset:]]
    scale = maxInstances / (len(entry) - 3 - offset)
    plt.plot(np.arange(0, maxInstances, scale), values, label=entry[0])

plt.legend()
plt.show()