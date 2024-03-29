# Importing Libraries 
import serial
import time
import soundcard as sc
import numpy as np
import math
import scipy
import matplotlib.pyplot as plt

default_speaker = sc.default_speaker()
print(default_speaker)

mics = sc.all_microphones(include_loopback=True)
print(mics)
default_mic = mics[0]

R = 48000
N = R // 10 # 10 updates per second.
T = 1 / R

ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 9600
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.open()
print(ser.name)
while not ser.read_until().decode().strip() == 'Listening.':
    pass
print("Connected.")

ser.write("I\n".encode())

while True:
    ser.flush()
    time.sleep(0.001)
    stereo = default_mic.record(samplerate=R, numframes=N)
    signal = np.sum(stereo, 1) / 2 # Average stereo signal.

    fourier = np.abs(scipy.fft.fft(signal, N))

    freq = scipy.fft.fftfreq(N, T)[:N//2]
    #plt.plot(freq, 2.0/N * np.abs(fourier[:N//2]))
    #plt.grid()
    #plt.show()

    bands = np.array_split(fourier[:N//2], 32)
    bands = np.max(bands, 1)
    banks = np.clip(bands, 0, 8)
    scale = (1600 / 8)
    scale = (800 / 8)
    bands = np.round(bands / scale).astype(int)
    graph = ''.join(np.asarray(bands.astype(str)))
    print(graph)
    ser.write(f"G {graph}\n".encode())