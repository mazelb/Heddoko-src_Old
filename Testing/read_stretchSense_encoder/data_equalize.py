def meaner(fo,ratio):
    sum=0
    for _ in range(ratio):
        val=fo.readline()
        val=int(val[0:4])
        sum=sum+val
    sum=float(sum)
    avg=sum/ratio
    avg=int(avg)
    return avg


count=0
count2=0
fname=raw_input('stretch sense file name (without extension)')
fname2=raw_input('encoder file name (without extension)')
fname3=raw_input('output file name (without extension)')
fo=open(fname,'r')
fo2=open(fname2,'r')

list1=[]

for line in fo:
    count+=1
    #list1.append(int(line[0:4]))

for line in fo2:
    count2+=1

ratio=count/count2


fo.close()
fo2.close()


fo=open(fname,'r')

for _ in range(count2):
    list1.append(meaner(fo, ratio))

fo.close()

fo3=open(fname3,'a')

for x in list1:
    fo3.write('%d\n'%x)

fo3.close()
