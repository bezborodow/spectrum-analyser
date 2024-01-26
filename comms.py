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

ser.write('1'.encode())

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

    banks = np.array_split(fourier[:N//2], 32)
    banks = np.max(banks, 1)
    #print(banks)
    banks = np.round(banks / (1600 / 8)).astype(int)
    #packet = bytearray()
    #packet.append(0xff)
    print(banks)
