import glob
import numpy as np
#import matplotlib.pyplot as plt

mt_files = glob.glob("../logs/mt_*.log")
mp_files = glob.glob("../logs/mp_*.log")

print(mt_files)
print(mp_files)

values = {}
for fn in mt_files:
    with open(fn, "r") as file:
        values[fn] = np.array([float(x.rstrip()) for x in file.readlines()])


for fn in mp_files:
    with open(fn, "r") as file:
        values[fn] = np.array([float(x.rstrip()) for x in file.readlines()])


print(values)


averages = {}
for fn in mt_files:
    averages[fn] = np.mean(values[fn])

for fn in mp_files:
    averages[fn] = np.mean(values[fn])



print(averages)

#plt.plot([44444, averages[mt_files[0]]], [9999999, averages[mt_files[1]]], 'k', lw=2)
#plt.plot([44444, averages[mp_files[0]]], [9999999, averages[mp_files[1]]], lw=2)
#plt.xlim(1000, 0.001)
#plt.show()
