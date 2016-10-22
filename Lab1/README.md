ECE382_Lab1
===========

##Documentation:
I absolutely need this first because I did get help. I followed the handout from Lecture 6 very closely when making this lab.  I also got help from C2C Jeremy Gruszka for the B Functionality.  I saw what the problem was, but didn't quite know how to make the code work the way it needed to.

##Lab1 - The Problem
A simple assembly language calculator

Basic Functionality:
ADD-OP flag is 0x11.
SUB_OP flag is 0x22.
CLR_OP flag is 0x44.
END_OP flag is 0x55.

B Functionality:
Setting results to a maximum of 255 and a minimum of 0 when they exceed those bounds.
   
A Functionality:
MUL_OP flag is 0x33.
   
##Implementation Plan

![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab1/master/PreLab1_Flowchart.JPG "Lab1 FLowChart")

#Code

##Initialization


main:
        mov.w #__STACK_END, SP
        mov.w #WDTPW|WDTHOLD, &WDTCTL

		mov.w #store_final_values, R12
		mov.w #array_operations, R5

		mov.b @R5+, R7
		mov.b @R5+, R8
		mov.b @R5+, R9

This section of the code is just the bare necessities to start the program.  It includes making the registers R7, R8, and R9 point to specific byte in the array

##Math Operations
####I have no clue why the code didn't show up correctly....

check11: ;Add_Op
		cmp.b #0x11, R8
		jnz check22
		mov.b R7, R10
		add.b R9, R10
		jc more_memory
		mov.b R10, 0(R12)
		inc.w R12
		jmp next

more_memory: ;B Func - More than 255
		add.b #0xff, 0(R12)
		inc.w R12
		jmp next

check22: ;Sub_Op
		cmp.b #0x22, R8
		jnz check44
		mov.b R7, R10
		sub.b R9, R10
		jl zero_to_memory
		mov.b R10, 0(R12)
		inc.w R12
		jmp next

zero_to_memory: ;B Func - Less than 0
		add.b #0x00, 0(R12)
		inc.w R12
		jmp next

check44: ;Clr_Op
		cmp.b #0x44, R8
		jnz next
		clr R10
		mov.b R10, 0(R12)
		inc.w R12
		inc.w R9
		inc.w R8
		inc.w R7
		jmp check11

R8 is intended to be the register that points to the math operation. 11 is the operation for addition, 22 for subtraction, and 44 for clearing.


next: ;Next set of numbers in array
		cmp.b #0x55, R9
		jz wait
		mov.b R8, R7
		mov.b R9, R8
		mov.b @R5+, R9
		jmp check11

wait: ;infinite loop
		jmp wait
		


"next" is simply to check for "55" in order to end the program.  If not, the registers are moved down the array to check for the next operation.  "wait" is to stick the CPU in an infinite loop.


#Thought Process

First thing to do was get an idea of have what the program needed to do, and how it would do it.  As shown above, I started with a flowchart just to see how everything should go.  The next part was making a very general code to see how the flowchart could happen.  My first attempt was in python saved as "PreLab1_Pseudocode1" in a link below.

![PseudoCode1](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab1/master/PreLab1_Pseudocode1.py "Pseudo1")

This code fails because of 2 assumptions it would make.  1st is that it assumes that 44 will only show up once and that 44 is not the first operation.

After looking back at some handouts given in class, I noticed that one of them practically had the assembly code for us.  Taking the layout and the idea of how it worked, I changed my pseudocode to the following:

![PseudoCode2](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab1/master/PreLab1_Pseudocode2.py "Pseudo2")

Although it did not run in python, it clearly painted a picture of how my code should run in assembly language (not including the A or B functionalities).  And with that in mind, I coded the final product in CCS:

![Final Program](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab1/master/Ruprecht_Lab1.asm "Final Program")
