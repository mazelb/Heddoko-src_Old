import numpy as np
import os
import tkinter as tk
from tkinter import filedialog


root = tk.Tk()
root.withdraw()
fn = filedialog.askopenfilename()


fo=input('output file name: ')

f = open(fo,'w')
nodlist = np.genfromtxt (fn, delimiter=",", dtype=str)
set=nodlist[0][3][8::]

if int(set) < 10:
    f.write('Set000')
elif int(set) < 100:
    f.write('Set00')
else:
    f.write('Set0')

f.write(set)
f.write(' ,9, FFFFFFFFFF , \n')


f.write('0,0,')
f.write(nodlist[0][1])
f.write('\n')

f.write('0,1,')
f.write(nodlist[1][1])
f.write('\n')

f.write('0,2,')
f.write(nodlist[2][1])
f.write('\n')

f.write('1,3,')
f.write(nodlist[3][1])
f.write('\n')

f.write('1,4,')
f.write(nodlist[4][1])
f.write('\n')

f.write('1,5,')
f.write(nodlist[5][1])
f.write('\n')

f.write('2,6,')
f.write(nodlist[6][1])
f.write('\n')

f.write('2,7,')
f.write(nodlist[7][1])
f.write('\n')

f.write('2,8,')
f.write(nodlist[8][1])
f.write('\n')

f.close()
