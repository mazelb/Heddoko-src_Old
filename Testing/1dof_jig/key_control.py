import msvcrt
import time
import serial
import sys

cport=raw_input("arduino com port (ex:30)\n")
cport='COM'+cport

ser = serial.Serial(cport, 9600)


print "Use q/a for fine control, w/s for med control and e/d for coarse control\n"

print "press ctrl+c to exit"
i=10

while True:
    #print "Doing a function"
    if msvcrt.kbhit():
        val=msvcrt.getch()
        if val=='a':
            i=i+1
            if i>179:
                i=180

        elif val=='q':
            i=i-1
            if i<1:
                i=0
        
        if val=='s':
            i=i+5
            if i>179:
                i=180

        elif val=='w':
            i=i-5
            if i<1:
                i=0

        if val=='d':
            i=i+10
            if i>179:
                i=180

        elif val=='e':
            i=i-10
            if i<1:
                i=0


        si=str(i)
    
        sendangle = si+'\n'
        ser.write(sendangle)
        ser.flush()
        time.sleep(.05)
