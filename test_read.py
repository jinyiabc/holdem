from pprint import pprint
# import test_constant_board as const

with open("test_an=0.5_bl=0.5_lim=1_r=2_liv=1_nr=5_pr=6.log", "r") as ins:
    array=array1=[]
    for idx, line in enumerate(ins):
        if idx == 94:
            array94 = line.split()
        if idx == 88:
            array88 = line.split()
        # if idx == 113:
        #     array113 = line.split()
        # if idx == 114:
        #     array114 = line.split()
        # if idx ==115:
        #     array115 = line.split()
        # if idx ==116:
        #     array116 = line.split()
        # if idx ==117:
        #     array117 = line.split()
        # if idx ==118:
        #     array118 = line.split()
        # if idx ==100:
        #     array100 = line.split()

# array88.append(array113[0])
# array88.append(array114[0])
# array88.append(array115[0])
# array88.append(array100[0])
# array94.append(array116[0])
# array94.append(array117[0])
# array94.append(array118[0])

new_array=[]
for idx in range(len(array88)):
    new_array.append(array88[idx][1:])
R1C_BB = set(new_array)
new_array1=[]
for idx in range(len(array94)):
    new_array1.append(array94[idx][1:])
R2C_BB = set(new_array1)


print "R1C for BB \n",R1C_BB
print(len(new_array),len(R1C_BB))
print "R2C+ for BB \n",R2C_BB
print(len(new_array1),len(R2C_BB))



with open("test.log", "r") as ins:
    array=array1=[]
    for idx, line in enumerate(ins):
        if idx == 135:
            R1C_SB = line.split()
        if idx == 141:
            CR1C_SB = line.split()
        if idx == 147:
            R2C_SB = line.split()
        if idx == 159:
            R3C_SB = line.split()
        if idx ==188:
            array188 = line.split()
        if idx ==189:
            array189 = line.split()
        if idx ==190:
            array190 = line.split()
R1C_SB.append(array189[0])
R2C_SB.append(array188[0])
R3C_SB.append(array190[0])

print "R1C for SB \n",R1C_SB
print "CR1C_SB for SB \n",CR1C_SB
print "R2C_SB for SB \n",R2C_SB
print "R3C_SB for SB \n",R3C_SB
