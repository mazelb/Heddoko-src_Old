import numpy as np
import os
import tkinter as tk
from tkinter import filedialog


root = tk.Tk()
root.withdraw()
fn = filedialog.askopenfilename()


fo=input('output file name: ')

f = open(fo,'w')
IMUlist = np.genfromtxt (fn, delimiter=",", dtype=str)
set=IMUlist[0][3][8::]

if int(set) < 10:
    f.write('Set000')
else:
    f.write('Set00')

f.write(set)
f.write(' ,9, FFFFFFFFFF , \n')



f.write('0,0,')
f.write(IMUlist[0][1])
f.write('\n')

f.write('0,1,')
f.write(IMUlist[1][1])
f.write('\n')

f.write('0,2,')
f.write(IMUlist[2][1])
f.write('\n')

f.write('1,3,')
f.write(IMUlist[3][1])
f.write('\n')

f.write('1,4,')
f.write(IMUlist[4][1])
f.write('\n')

f.write('1,5,')
f.write(IMUlist[5][1])
f.write('\n')

f.write('2,6,')
f.write(IMUlist[6][1])
f.write('\n')

f.write('2,7,')
f.write(IMUlist[7][1])
f.write('\n')

f.write('2,8,')
f.write(IMUlist[8][1])
f.write('\n')

f.close()
