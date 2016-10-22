#C2C Nathan Ruprecht
#Pre-Lab1: Pseudocode
#8 September 2014
#ECE 382

array_operations = 14, 11, 32, 22, 8, 44, 4, 11, 8, 55
#11 adds
#22 subtracts
#44 clears
#55 ends

#initialize registers
i = 0
R7 = 0
R8 = 0
R9 = 0
R12 = 0

#moving registers into place
R5 = array_operations
R7 = array_operations[i]
R8 = array_operations[i+1]
R9 = array_operations[i+2]

#check11 - Add
if (R8 == 11):
    R10 = R7
    R10 += R9
    R12 = R10

#check22 - Subtract
elif (R8 == 22):
    R10 = R7
    R10 = R10 - R9
    R12 = R10

#check44 - Clear
elif (R8 == 44):
    R10 = 0
    R12 = R10
    R9 = array_operations[i=4]
    R8 = array_operations[i+3]
    R7 = array_operations[i+2]

#check55 - End
elif (R8 == 55)
    #infinite loop
else:
    R7 = R8
    R8 = R9
    R9 = array_operations[i+5]
    
#infinitie loop

