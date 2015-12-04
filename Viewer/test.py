# test.py
import subprocess
import sys #args

f = open("result3.po", 'r')
f2 = open("block_rwbs1.dat", 'w') # block to rwbs, I/O Count
f3 = open("block_rwbs2.dat", 'w') # block to rwbs, I/O Count, Percentage(%)
f4 = open("block_rwbs3.dat", 'w') # block to rwbs, I/O Size
f5 = open("block_rwbs4.dat", 'w') # block to rwbs, I/O Size, Percentage(%)

#f3 = open("block_action.dat", 'w')
#action = [0 for _ in range(15)] # action type list initialization
#lst = [[0 for col in range(3)] for row in range(15)] # for action
lst = [[0 for col in range(3)] for row in range(3)] # Block to rwbs(I/O Count)
lst2 = [[0 for col in range(3)] for row in range(15)] # Block to action
lst3 = [[0 for col in range(3)] for row in range(3)] # Block to rwbs(I/O Size)
action = {"Q": 0, "M": 1, "F": 2, "G": 3, "S": 4, "R": 5, "D": 6, "C": 7, "P": 8, "U": 9, "UI": 10, "I": 11, "X": 12, "B": 13, "A": 14}
rwbs = {"R": 0, "W": 1, "N": 2}
while 1:
	line  = f.readline().replace("\n", "")
	data = line.split("\t")
	if not line: break
	#if data[7] == "D" :
	#	data[8] = data[8].replace("\n","")
	#else : data[7] = data[7].replace("\n","")
	#print(line)
	#print(data)
	"""
	if data[1] == "Q": action = 0
        elif data[1] == "M": action = 1
        elif data[1] == "F": action = 2
        elif data[1] == "G": action = 3
        elif data[1] == "S": action = 4
        elif data[1] == "R": action = 5
        elif data[1] == "D": action = 6
        elif data[1] == "C": action = 7
        elif data[1] == "P": action = 8
        elif data[1] == "U": action = 9
        elif data[1] == "UI": action = 10
        elif data[1] == "I": action = 11
        elif data[1] == "X": action = 12
        elif data[1] == "B": action = 13
	elif data[1] == "A": action = 14
	"""
	#print(action[data[1]])
	# RWBS
	if data[2].find("R") >= 0:
		tmp = "R"
	elif data[2].find("W") >= 0:
		tmp = "W"
	elif data[2] == "N":
		tmp = "N"
	#print(tmp+':'+str(rwbs[tmp]))

	if data[7] == "D":
		lst[rwbs[tmp]][0] += 1 # I/O Count
		lst3[rwbs[tmp]][0] += int(data[4])/2 # data[4] : I/O Size(512byte)
	elif data[7] == "J":
		lst[rwbs[tmp]][1] += 1
		lst3[rwbs[tmp]][1] += int(data[4])/2
	elif data[7] == "M":
		lst[rwbs[tmp]][2] += 1
		lst3[rwbs[tmp]][2] += int(data[4])/2
	else :
		print("ERROR!-rwbs")
	#print(lst)

	# action
	if data[7] == "D":
                lst2[action[data[1]]][0] += 1
        elif data[7] == "J":
                lst2[action[data[1]]][1] += 1
        elif data[7] == "M":
                lst2[action[data[1]]][2] += 1
        else :
                print("ERROR!-action")

	"""
	if data[1] == "Q":
		action[0] += 1
	elif data[1] == "M":
		action[1] += 1
	elif data[1] == "F":
		action[2] += 1
	elif data[1] == "G":
		action[3] += 1
	elif data[1] == "S":
                action[4] += 1
	elif data[1] == "R":
                action[5] += 1
	elif data[1] == "D":
                action[6] += 1
	elif data[1] == "C":
                action[7] += 1
	elif data[1] == "P":
                action[8] += 1
	elif data[1] == "U":
                action[9] += 1
	elif data[1] == "UI":
                action[10] += 1
	elif data[1] == "I":
                action[11] += 1
	elif data[1] == "X":
                action[12] += 1
	elif data[1] == "B":
                action[13] += 1
	elif data[1] == "A":
                action[14] += 1
	print(action)
	"""
#f2.write(str(lst))
# I/O Count
order = 0
f2.write("\tD\tJ\tM\n")
for i in lst:
	f2.write(rwbs.keys()[rwbs.values().index(order)]+'\t'+str(i[0])+'\t'+str(i[1])+'\t'+str(i[2])+'\n')
	order += 1
# I/O Count - Percentage(%)
order = 0
f3.write("\tD\tJ\tM\n")
for i in lst:
	rowSums = sum(i, 0.0)
	if not rowSums == 0:
        	f3.write(rwbs.keys()[rwbs.values().index(order)]+":"+str(int(rowSums))+'\t'+str(i[0]*100/rowSums)+'\t'+str(i[1]*100/rowSums)+'\t'+str(i[2]*100/rowSums)+'\n')
        else :
		f3.write(rwbs.keys()[rwbs.values().index(order)]+":"+str(int(rowSums))+'\t'+str(i[0])+'\t'+str(i[1])+'\t'+str(i[2])+'\n')
	order += 1
# I/O Size
order = 0
f4.write("\tD\tJ\tM\n")
for i in lst3:
        f4.write(rwbs.keys()[rwbs.values().index(order)]+'\t'+str(i[0])+'\t'+str(i[1])+'\t'+str(i[2])+'\n')
        order += 1
# I/O Size - Percentage(%)
order = 0
f5.write("\tD\tJ\tM\n")
for i in lst3:
        rowSums = sum(i, 0.0)
        if not rowSums == 0:
                f5.write(rwbs.keys()[rwbs.values().index(order)]+":"+str(int(rowSums))+"KB"+'\t'+str(i[0]*100/rowSums)+'\t'+str(i[1]*100/rowSums)+'\t'+str(i[2]*100/rowSums)+'\n')
        else :
                f5.write(rwbs.keys()[rwbs.values().index(order)]+":"+str(int(rowSums))+"KB"+'\t'+str(i[0])+'\t'+str(i[1])+'\t'+str(i[2])+'\n')
        order += 1
"""
order = 0
f3.write("\tD\tJ\tM\n")
for i in lst2:
        f3.write(action.keys()[action.values().index(order)]+'\t'+str(i[0])+'\t'+str(i[1])+'\t'+str(i[2])+'\n')
        order += 1
"""

exe = 'Rscript rscript.r'
# set the Graph Size Option
if sys.argv.count('-s') == 1:
        ind = sys.argv.index('-s') + 1
        x_length = sys.argv[ind] # unit : inch
        y_length = sys.argv[ind+1]
        exe = exe + ' -s ' + x_length + ' ' + y_length

# set the Output file option
if sys.argv.count('-o') == 1:
        ind = sys.argv.index('-o') + 1
        out = sys.argv[ind].split('.')
        exe = exe +' -o '+ sys.argv[ind] +' ' + out[1] # out[1] : file_extension

# set the Y-axis Option
if sys.argv.count('-y') == 1:
	ind2 = sys.argv.index('-y') + 1
	if sys.argv[ind2] == 's' or sys.argv[ind2] == 'c' or sys.argv[ind2] == 'all':
		exe = exe + ' -y ' + sys.argv[ind2]
	else:
		print('Error!! : Invalid argvs(-y)....')
		sys.exit()

# set the input option
if sys.argv.count('-i') == 1:
        ind = sys.argv.index('-i') + 1
        name = sys.argv[ind]
	time = sys.argv[ind+1]
	exe = exe + ' -i '+ name + ' ' + time

# File Close
f.close()
f2.close()
f3.close()
f4.close()
f5.close()

#print(exe)
# Execute the R script.
subprocess.call(exe, shell=True)
