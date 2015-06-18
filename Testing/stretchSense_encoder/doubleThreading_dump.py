import threading

import serial
from time import sleep
import sys



def avg(list):
    sum = 0
    for elm in list:
        sum += elm
    avg=sum/(len(list)*1.0)
    return  avg




def enwrite(ser, fname, flag):
    
    #flag=0
    while True:
        fo = open(fname, "a")
        if ser.inWaiting():
            rawdata=ser.readline();
            flag=1
            sleep(1)
            fo.write(rawdata)
            print "encoder value: %s \n" %rawdata
            fo.close()

def sswrite(ser, fname, flag):
    holder=[]
    while True:
        #fo = open(fname, "a")
        ser.write('#s\r\n')
        ser.flush()
        if ser.inWaiting():
            rawdata=ser.readline();
            ch1Val=rawdata[1:5]
            
            if ch1Val[0:4]=='Feed' or ch1Val[0:4]=='feed':
                pass
            else:
                ch1Val=float(ch1Val)
                holder.append(ch1Val)
                print "ss value: %s \n" %ch1Val
                #fo.close()
                
            if flag==1:
                fo.open(fname, "a")
                send=avg(holder)
                fo.write(send)
                fo.close()
                holder=[]
                flag=0

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


    flag=0
    
    print 'strating'


    try:             
            
        t=threading.Thread(target=sswrite, args=(ser1,fname,flag))
        q=threading.Thread(target=enwrite, args=(ser2,fname2,flag))
        q.start()
        t.start()
    
    except KeyboardInterrupt:
        ser1.write('#t\r\n')
        ser1.close()
        fo.close()

        ser2.close()
        fo2.close()
