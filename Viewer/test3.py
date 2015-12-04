# test3.py
import subprocess
import sys # args

f = open("file_info2.txt", 'r')
f2 = open("result3.po", 'r')
f3 = open("file_rwbs1.dat", 'w') # file to rwbs, I/O Count
f4 = open("file_rwbs2.dat", 'w') # file to rwbs, I/O Count, Percentage(%)
f5 = open("file_rwbs3.dat", 'w') # file to rwbs, I/O Size
f6 = open("file_rwbs4.dat", 'w') # file to rwbs, I/O Size, Percentage(%)

# make the empty dictionary
file_type = dict() # file_name to file_type
file_num = dict() # file_type to file_number

#lst = [[0 for col in range(16)] for row in range(15)]
lst = [[0 for col in range(16)] for row in range(3)] # Block to rwbs(I/O Count)
lst2 = [[0 for col in range(16)] for row in range(3)] # Block to rwbs(I/O Size)
action = {"Q": 0, "M": 1, "F": 2, "G": 3, "S": 4, "R": 5, "D": 6, "C": 7, "P": 8, "U": 9, "UI": 10, "I": 11, "X": 12, "B": 13, "A": 14}
rwbs = {"R": 0, "W": 1, "N": 2}
# File Type Define
size = 0
while 1:
	line = f.readline()
	line = line.replace("\n", "")
	if not line : break
	data = line.split(" ")
	for i in data:
		if not i == data[0]:
			file_type[i] = data[0]
	file_num[data[0]] = size
	size += 1
file_num['ETC_FILE'] = size
#print(file_type)

# General IO => File type
while 1:
	line = f2.readline()
	line = line.replace("\n", "")
	if not line : break
	data = line.split("\t")
	leng = len(data[8])
	fname = [0 for _ in range(300)]
	temp  = data[8]
	
	# RWBS
        if data[2].find("R") >= 0:
                tmp = "R"
        elif data[2].find("W") >= 0:
                tmp = "W"
        elif data[2] == "N":
                tmp = "N"

	cur_type = 'ETC_FILE'
	for i in file_type.keys():
		if i.find('$') != -1:
			tmp2 = i.split('$')
			#print(tmp2)
			ext = tmp2[0]
			num = int(tmp2[1])
			index = data[8].find(ext)
			if index > 0 and (leng - index) == (len(ext) + num) and (data[8][index-1] == '.' or data[8][index-1] == '-' or data[8][index-1] == '_'):
				cur_type = file_type[i]
		else:
			index = data[8].find(i)
		#print(data[8]+','+i+','+str(leng)+','+str(data[8].find(i))+','+str(len(i)))
			if index >= 0 and (leng - index) == len(i) and (data[8][index-1] == '.' or data[8][index-1] == '-' or data[8][index-1] == '_'):
				cur_type = file_type[i]	
	#print(data[8]+' : '+cur_type)
	#f3.write(data[8]+' : '+cur_type+'\n')
	if not cur_type == 'NONE_FILE':
		#lst[action[data[1]]][file_num[cur_type]] += 1
		lst[rwbs[tmp]][file_num[cur_type]] += 1
		lst2[rwbs[tmp]][file_num[cur_type]] += int(data[4])/2 # data[4] : I/O Size(512byte->1KB)

# Write the Parsing Data - I/O Count
# Columns
order = 0
for i in range(0, size+1):
        f3.write('\t'+file_num.keys()[file_num.values().index(order)])
        order += 1
f3.write('\n')
# Rows + Elements
order = 0
for i in lst:
        f3.write(rwbs.keys()[rwbs.values().index(order)])
        for index in range(0, size+1):
                f3.write('\t'+str(i[index]))
        f3.write('\n')
        order += 1
#print(file_num)

# Write the Parsing Data - I/O Count - Percentage(%)
# Columns
order = 0
for i in range(0, size+1):
        f4.write('\t'+file_num.keys()[file_num.values().index(order)])
        order += 1
f4.write('\n')
# Rows + Elements
order = 0
for i in lst:
	rowSums = sum(i, 0.0)
        f4.write(rwbs.keys()[rwbs.values().index(order)]+":"+str(int(rowSums)))
        for index in range(0, size+1):
		if not rowSums == 0:
                	f4.write('\t'+str((i[index])*100/rowSums))
		else:
			f4.write('\t'+str(i[index]))
        f4.write('\n')
        order += 1
# Write the Parsing Data - I/O Size(1KB)
# Columns
order = 0
for i in range(0, size+1):
        f5.write('\t'+file_num.keys()[file_num.values().index(order)])
        order += 1
f5.write('\n')
# Rows + Elements
order = 0
for i in lst2:
        f5.write(rwbs.keys()[rwbs.values().index(order)])
        for index in range(0, size+1):
                f5.write('\t'+str(i[index]))
        f5.write('\n')
        order += 1
#print(file_num)

# Write the Parsing Data - I/O Size - Percentage(%)
# Columns
order = 0
for i in range(0, size+1):
        f6.write('\t'+file_num.keys()[file_num.values().index(order)])
        order += 1
f6.write('\n')
# Rows + Elements
order = 0
for i in lst2:
        rowSums = sum(i, 0.0)
        f6.write(rwbs.keys()[rwbs.values().index(order)]+":"+str(int(rowSums))+"KB") # 1024byte = 1KB
        for index in range(0, size+1):
                if not rowSums == 0:
                        f6.write('\t'+str((i[index])*100/rowSums))
                else:
                        f6.write('\t'+str(i[index]))
        f6.write('\n')
        order += 1

"""
order = 0
for i in lst:
        f4.write(action.keys()[action.values().index(order)])
        for index in range(0, size+1):
                f4.write('\t'+str(i[index]))
        f4.write('\n')
        order += 1
print(file_num)
"""

exe = 'Rscript rscript3.r'

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
f6.close()

#print(exe)
# Execute the R script.
subprocess.call(exe, shell=True)
