from time import sleep
import serial
import sys
ser = serial.Serial('COM30', 9600) # Establish the connection on a specific port


fo = open("data.txt", "r")
try:
     while True:
          
          if(ser.inWaiting()):     
               print ser.readline()
          #sendangle=raw_input('angle:')
          #sendangle=sendangle+"\r\n"

          sendangle=fo.readline()

          if(sendangle!=""):
               sendangle=sendangle.replace("\t",",")
               sendangle=sendangle.replace("\n",",\r\n")
               
               ser.write(sendangle) # Convert the decimal number to ASCII then send it to the Arduino
               ser.flush()
          
               sleep(.1) # Delay for two tenths of a second
               ser.flushOutput()
          
               if(ser.inWaiting()):
                    print ser.readline() # Read the newest output from the Arduino
          else:
               fo.close()
          #ser.close()
          #ser.open()

except KeyboardInterrupt:
     ser.close()
