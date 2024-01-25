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
default_mic = mics[1]

R = 48000
N = R // 10 # 10 updates per second.
T = 1 / R

ser = serial.Serial()
ser.port = 'COM3'
ser.baudrate = 115200
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
ser.timeout = .1
ser.dsrdtr = False
ser.rtscts = False
ser.xonxoff = False
ser.open()
print(ser.is_open)
print(ser.name)
packet = bytearray()
packet.append(0xff)
packet.append(0x80)
ser.write(b'\xff')
ser.write(packet)
time.sleep(0.05) 
ser.flush()
time.sleep(0.05) 
print(ser.is_open)
ser.close()
print(ser.is_open)
exit()

while True:
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
    #ser.write(packet)
    #ser.write(b'\xff')
    #ser.write(bytes(banks))
    print(banks)
