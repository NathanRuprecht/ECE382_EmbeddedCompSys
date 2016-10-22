#C2C Nathan Ruprecht
#Pre-Lab1: Pseudocode
#8 September 2014
#ECE 382

num = 14, 11, 32, 22, 8, 44, 4, 11, 8, 55
#11 adds
#22 subtracts
#44 clears
#55 ends

i = 0
R7 = 0
R8 = 0
R9 = 0
R12 = 0

#looping until the 1st operation
while (R12 == 0):
    
    R9 = num[i+1]
    R8 = num[i]
    R7 = num[i-1]
    
    if (R8 == 11):
        R12 = R7 + R9
        print(R12)
    if (R8 == 22):
        R12 = R7 - R9
        print(R12)
    if (R8 == 44):
        R12 = 0
        print(R12)
        
    i += 1

#looping until 0x44 clears
while (R12 != 0):
    
    R9 = num[i+1]
    R8 = num[i]
    
    if (R8 == 11):
        R12 = R12 + R9
        print(R12)
    if (R8 == 22):
        R12 = R12 - R9
        print(R12)
    if (R8 == 44):
        R12 = 0
        print(R12)
        
    i += 1

#looping from 0x44 to 0x55
while (R9 != 55):
    
    R9 = num[i+1]
    R8 = num[i]
    R7 = num[i-1]
    
    if (R8 == 11):
        R12 = R7 + R9
        print(R12)
    if (R8 == 22):
        R12 = R7 - R9
        print(R12)
    if (R8 == 44):
        R12 = 0
        print(R12)
        
    i += 1
