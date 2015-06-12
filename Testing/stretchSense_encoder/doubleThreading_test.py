import threading

import serial
from time import sleep
import sys



def enwrite(ser, fname):
    while True:
        fo = open(fname, "a")
        rawdata=ser.readline();
        fo.write(rawdata)
        print "encoder value: %s \n" %rawdata
        fo.close()

def sswrite(ser, fname):
    while True:
        fo = open(fname, "a")
        ser.write('#s\r\n')
        rawdata=ser.readline();
        ch1Val=rawdata[1:5]
        ch1Val=ch1Val+'\n'
        fo.write(ch1Val)
        print "ss value: %s \n" %ch1Val
        fo.close()

if __name__ == '__main__':


    fname=raw_input("ss output file name (without entension):\n")
    fname=fname+'.txt'
    cport=raw_input("stretch sense com port (ex:)\n")
    cport='COM'+cport
    ser1 = serial.Serial(cport, 115200)

    fname2=raw_input("en output file name (without entension):\n")
    fname2=fname2+'.txt'
    cport2=raw_input("arduino com port (ex:)\n")
    cport2='COM'+cport2
    ser2 = serial.Serial(cport2, 115200)

    
    print 'strating'


    try:             
            
        t=threading.Thread(target=sswrite, args=(ser1,fname))
        q=threading.Thread(target=enwrite, args=(ser2,fname2))
        t.start()
        q.start()
    
    except KeyboardInterrupt:
        ser1.write('#t\r\n')
        ser1.close()
        fo.close()

        ser2.close()
        fo2.close()
