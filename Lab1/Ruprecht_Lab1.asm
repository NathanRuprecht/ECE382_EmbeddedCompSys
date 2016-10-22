;-------------------------------------------------------------
; Lab 1 - Assembly Language - A Simple Calculator
; C2C Nathan Ruprecht, USAF / 7 Sept 2014 / 10 Sept 2014
;
;Program works for all cases tested
;
; This program is a demonstration of using the CCS IDE to
; program, assemble, flash, and debug the MSP430.
;-------------------------------------------------------------
	.cdecls C,LIST,"msp430.h"	; BOILERPLATE	Include device header file
 	.text						; BOILERPLATE	Assemble into program memory
	.retain						; BOILERPLATE	Override ELF conditional linking and retain current section
	.retainrefs					; BOILERPLATE	Retain any sections that have references to current section
	.global main				; BOILERPLATE	Project -> Properties and select the following in the pop-up
								; Build -> Linker -> Advanced -> Symbol Management
								;    enter main into the Specify program entry point... text box
	.data
	.bss store_final_values, 0x40
	.text

array_operations:	.byte	0x14, 0x11, 0x32, 0x22, 0x08, 0x44, 0x04, 0x11, 0x08, 0x55

;-------------------------------------------------------------------------------
;           						main
;-------------------------------------------------------------------------------
main:
        mov.w #__STACK_END, SP
        mov.w #WDTPW|WDTHOLD, &WDTCTL

		mov.w #store_final_values, R12
		mov.w #array_operations, R5

		mov.b @R5+, R7
		mov.b @R5+, R8
		mov.b @R5+, R9

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

next: ;Next set of numbers in array
		cmp.b #0x55, R9
		jz wait
		mov.b R8, R7
		mov.b R9, R8
		mov.b @R5+, R9
		jmp check11

wait: ;infinite loop
		jmp wait

;-------------------------------------------------------------------------------
;           System Initialization
;-------------------------------------------------------------------------------
	.global __STACK_END				; BOILERPLATE
	.sect 	.stack					; BOILERPLATE
	.sect   ".reset"				; BOILERPLATE		MSP430 RESET Vector
	.short  main					; BOILERPLATE
