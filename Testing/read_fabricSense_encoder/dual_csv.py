import serial
from time import sleep
import sys

print "make sure fabric sensor connected to channel 1\n"


fname2=raw_input("output file name (without entension):\n")
fname2=fname2+'.txt'

cport=raw_input("fabric sensor com port (ex49:)\n")
cport='COM'+cport
cport2=raw_input("arduino com port (ex:55)\n")
cport2='COM'+cport2
ser1 = serial.Serial(cport, 115200)
ser2 = serial.Serial(cport2, 115200, timeout=0)

fo2 = open(fname2, "a")

ser1.write('#s\r\n')
try:
    while True:
        
        rawdata2=ser2.readline()
        

        rawdata=ser1.readline()
         
        ch1Val=rawdata[1:5]


        if rawdata2 != '' and len(rawdata2)==8 and rawdata2 != '\r\n' and rawdata2 != 'Feed\r\n':
            printval = rawdata2[:-2]+'\t'+ch1Val+'\r\n'
            print printval
            fo2.write(printval)

        if rawdata=="":
            ser1.close()
            fo2.close()
            sys.exit()
            
            

except KeyboardInterrupt:
    ser1.write('#t\r\n')
    ser1.close()
    ser2.close()
    fo2.close()
    
