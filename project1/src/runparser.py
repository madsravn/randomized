import re
fdcounts = {}
lastn = 100
infile = open("biglog.txt","rt")
for line in infile:
	m = re.match(r'1:.*n: (\d*).*depth: (\d*).*comps: (\d*)', line)
	if(m):
		n = int(m.group(1))
		depth =m.group(2)
		comps = int(m.group(3))
		if(n != lastn):
			print("for n: %d" % lastn)
			for k,v in fdcounts.items():
				print("%s\t%s" % (k,v))
			lastn = n
			fdcounts = {}
		try:
			fdcounts[depth] = int(fdcounts[depth]+1)
		except KeyError:
			fdcounts[depth] =1

	m = re.match(r'9:', line)
	if(m):
                print(line)
print("for n: %d" % lastn)
for k,v in fdcounts.items():
	print("%s\t%s" % (k,v))
