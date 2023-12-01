"""
Python script using NumPy to compute Fourier frequencies with numpy.fft.fftfreq.
Adjust the array size to analyze different matrices.
"""

import numpy as np
import matplotlib.pyplot as plt

# Number of data points
n = 1000

# Sample spacing
d = 0.01

# Generate a simple signal (e.g., a sum of two sinusoids)
t = np.linspace(0.0, n * d, n, endpoint=False)
signal = 0.5 * np.sin(2.0 * np.pi * 5.0 * t) + 0.3 * np.sin(2.0 * np.pi * 20.0 * t)

# Compute FFT
fft_result = np.fft.fft(signal)

# Generate frequencies
frequencies = np.fft.fftfreq(n, d)

# Plot the original signal and its frequency spectrum
plt.subplot(2, 1, 1)
plt.plot(t, signal)
plt.title('Original Signal')

plt.subplot(2, 1, 2)
plt.plot(frequencies, np.abs(fft_result))
plt.title('Frequency Spectrum')
plt.xlabel('Frequency (Hz)')

plt.tight_layout()
plt.show()
