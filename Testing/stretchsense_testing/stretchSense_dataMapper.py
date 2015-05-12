import os

def sliceup(list1, chunk):
	list2=zip(*[iter(list1)]*chunk)
	remainder=len(list1)%chunk
	listc=[]
	for x in range(len(list2)):
		listc.append(list(list2[x]))
	
	if remainder==0:
		pass

	elif remainder <= (chunk/2):
		x=1
		while x <= remainder:
			listc[-1].insert(chunk,list1[-x])
			x=x+1

	else:
		x=1
		listd=[]
		while x <= remainder:
			listd.insert(0,list1[-x])
			x=x+1
		listc.append(listd)
	return listc

def meaner(list1):
	resultlist=[]
	x=len(list1)
	y=0
	while y<x:
		temp=(float(sum(list1[y]))/len(list1[y]))
		temp=round(temp)
		temp=int(temp)
		resultlist.append(temp)
		y=y+1
	return resultlist



path=raw_input('path(remember to escape backslashes):\n')


fname=raw_input('input file name(include extension):\n')


fname2=raw_input('output file name(include extension):\n')

splitsize=raw_input('desired split size(for averaging):\n')

splitsize=int(splitsize)

os.chdir(path)


lista=[]


with open(fname, "r") as fo:
	for line in fo:
		val=line[0:-1]
		val=int(val)
		lista.append(val)


max=max(lista)


min=min(lista)


delta=max-min


ratio=180.0/delta



listb=[]

for x in lista:
	val=0
	val=(x-min)*ratio
	val=round(val)
	listb.append(val)



listb=sliceup(listb,splitsize)

listb=meaner(listb)


with open(fname2, "a") as fo:
	y=0
	for x in listb:
		x=int(x)
		if (abs(x-y))<2:
			pass
		else:
			x=str(x)
			fo.write(x+'\n')
			x=int(x)
		y=x

