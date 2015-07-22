import serial
from time import sleep
import sys

print "make sure sensor connected to channel 1\n"
fname=raw_input("output file name (without entension):\n")
fname=fname+'.txt'
cport=raw_input("stretch sense com port (ex:)\n")
cport='COM'+cport
ser1 = serial.Serial(cport, 115200)
fo = open(fname, "a")
ser1.write('#s\r\n')

try:
    while True:
        rawdata=ser1.readline();
        #foo=raw_input("21+ digit number:")

        ch1Val=rawdata[1:5] 
        #ch2Val=rawdata[5:9]
        #ch3Val=rawdata[9:13]
        #ch4Val=rawdata[13:17]
        #ch5Val=rawdata[17:21] 

        ch1Val=ch1Val+'\n' 
        #ch2Val=ch2Val+','
        #ch3Val=ch3Val+','
        #ch4Val=ch4Val+','
        #ch5Val=ch5Val+',' 

        fo.write(ch1Val) 
        #fo.write(ch2Val)
        #fo.write(ch3Val)
        #fo.write(ch4Val)
        #fo.write(ch5Val) 

        #sleep(.1)

        if rawdata=="":
            ser1.close()
            fo.close()
            sys.exit()
            

except KeyboardInterrupt:
    ser1.write('#t\r\n')
    ser1.close()
    fo.close()
    
