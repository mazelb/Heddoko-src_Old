import msvcrt
import time
import serial
import sys
ser = serial.Serial('COM30', 9600)

i,j,k=0,0,0

while True:
    #print "Doing a function"
    if msvcrt.kbhit():
        val=msvcrt.getch()
        if val=='w':
            i=i+5
            if i>179:
                i=180

        elif val=='s':
            i=i-5
            if i<1:
                i=0

        elif val=='e':
            j=j+5
            if j>179:
                j=180

        elif val=='d':
            j=j-5
            if j<1:
                j=0

        elif val=='r':
            k=k+5
            if k>179:
                k=180

        elif val=='f':
            k=k-5
            if k<1:
                k=0

        si=str(i)
        sj=str(j)
        sk=str(k)
        sendangle = si+','+sj+','+sk+'\n'
        ser.write(sendangle)
        ser.flush()
        time.sleep(.05)
