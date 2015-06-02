import serial
import sys
import time

cport=raw_input("arduino com port (ex:30)\n")
cport='COM'+cport

ser = serial.Serial(cport, 9600)

while True:
    angle=raw_input("angle:\n")
    angle=angle+"\n"
    ser.write(angle)
    ser.flush()
    time.sleep(0.05)
