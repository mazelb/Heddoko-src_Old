import serial
from time import sleep
import sys

print "make sure sensor connected to channel 1\n"
#fname=raw_input("Stretch output file name (without entension):\n")
#fname=fname+'.txt'

fname2=raw_input("Encoder output file name (without entension):\n")
fname2=fname2+'.txt'

cport=raw_input("stretch sense com port (ex:)\n")
cport='COM'+cport
cport2=raw_input("arduino com port (ex:)\n")
cport2='COM'+cport2
ser1 = serial.Serial(cport, 115200)
ser2 = serial.Serial(cport2, 115200, timeout=0)

#fo = open(fname, "a")
fo2 = open(fname2, "a")

ser1.write('#s\r\n')

myAss = False
scratch = 0;

try:
    while True:
        
        rawdata2=ser2.readline()
        

        rawdata=ser1.readline()
         
        ch1Val=rawdata[1:5]
        #ch2Val=rawdata[5:9]
        #ch3Val=rawdata[9:13]
        #ch4Val=rawdata[13:17]
        #ch5Val=rawdata[17:21] 

        #ch1Val=ch1Val+'\r\n'
        #list1.append(ch1Val)
        #ch2Val=ch2Val+','
        #ch3Val=ch3Val+','
        #ch4Val=ch4Val+','
        #ch5Val=ch5Val+',' 

        #fo.write(ch1Val) 
        #fo.write(ch2Val)
        #fo.write(ch3Val)
        #fo.write(ch4Val)
        #fo.write(ch5Val)
        #bothvals=ch1Val+'\t'+rawdata2

        if rawdata2 != '' and len(rawdata2)==8 and rawdata2 != '\r\n' and rawdata2 != 'Feed\r\n':
            printval = rawdata2[:-2]+'\t'+ch1Val+'\r\n'
            print printval
            fo2.write(printval)
        

     

        #sleep(.1)
        
        #print rawdata2
        if rawdata=="":
            ser1.close()
            fo2.close()
            sys.exit()
            
        #ser1.flush()
        #ser1.flushInput()
        #ser1.flushOutput()
        
        #ser2.flush()
        #ser2.flushInput()
        #ser2.flushOutput()
            

except KeyboardInterrupt:
    #while ser1.inWaiting():
     #   sleep(.1)
    #while ser2.inWaiting():
     #   sleep(.1)
    ser1.write('#t\r\n')
    ser1.close()
    ser2.close()
    #fo.close()
    fo2.close()
    
