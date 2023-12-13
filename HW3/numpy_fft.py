"""
Python script using NumPy to compute Fourier frequencies with numpy.fft.fftfreq.
Given arg as the array size
"""
import numpy as np
import matplotlib.pyplot as plt
import csv
import sys

# the only argument is the maxtric size n
n = int(sys.argv[1])
print(f'size of the matirx: {n}')

# compute frequencies based on the size
frequencies = np.fft.fftfreq(n)
frequencies = np.round(frequencies, decimals=4) * n

# save the frequncies as 2-D matrix to be compared with the results from fft computeFrequencies
result_matrix = np.empty((n, n), dtype=tuple)
for i in range(n):
    for j in range(n):
        result_matrix[i, j] = (frequencies[i], frequencies[j])

print('numpy-computed frequencies:')
print(result_matrix) 

with open('numpy_computed_frequencies.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerows(result_matrix)
