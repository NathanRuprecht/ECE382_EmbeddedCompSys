ECE382_Lab3 - SPI - "I/O"
===========
##Lab3 Purpose
Understand how the MSP430 interacts with the Nokia1202 LCD.  Be able to code to use the Nokia1202.

####Basic Functionality:

Create a block on the LCD that is 8x8 pixels.  The location of the block must be passed into the subroutine via R12 and R13.

####A Functionality:

Move the 8x8 pixel block on block in the direction of the pressed button (up, down, left, right).
   
##Preliminary Design

####Mega Prelab

The majority of this lab was familuarizing ourselves with the Nokia display and getting comfortable with using it along side our MSP430.  The first thing we did was to look up the schematic for the Nokia 1202 LCD BoosterPack and fill in a given table.  The purpose of the table is to just introduce us to the display, the different pins we will be using, and how each pin works.  Below is the completed table for that info:

|__Name__|__Pin #__|__Type__|__PxDIR__|__PxDIR__|__PxDIR__|
|:-----|:-----|:-----|:-----|:-----|:-----|
|GND|`20`|Power|`-`|`-`|`-`|
|RST|`8`|Output|`1`|`0`|`1`|
|P1.4|`6`|Output|`1`|`0`|`1`|
|MOSI|`15`|Output|`1`|`0`|`1`|
|SCLK|`7`|Output|`1`|`0`|`1`|
|VCC|`1`|Power|`-`|`-`|`-`|
|S1|`9`|Input|`0`|`1`|`1`|
|S2|`10`|Input|`0`|`1`|`1`|
|S3|`11`|Input|`0`|`1`|`1`|
|S4|`12`|Input|`0`|`1`|`1`|

The next portion of the Mega PreLab was to understand how the pins were being used.  We were given the below "mystery code" and had to figure out what the labels A, B, C, D stood for as far as registers.  The table below the code is complete with the registers corresponding tot he labels.

               mov.b   #LCD1202_CS_PIN|LCD1202_BACKLIGHT_PIN|LCD1202_SCLK_PIN|LCD1202_MOSI_PIN, & A
               mov.b   #LCD1202_CS_PIN|LCD1202_BACKLIGHT_PIN|LCD1202_SCLK_PIN|LCD1202_MOSI_PIN, & B
               mov.b   #LCD1202_RESET_PIN, & C
               mov.b   #LCD1202_RESET_PIN, & D


|__Mystery Label__|__Register__|
|:-----|:-----|
|A|P1DIR|
|B|P1OUT|
|C|P2DIR|
|D|P2OUT|

For the next bit of given code, we were told to identify how the code-snippet configures that pin and what function is realized by that setting.

                bis.b   #UCCKPH|UCMSB|UCMST|UCSYNC, &UCB0CTL0
                bis.b   #UCSSEL_2, &UCB0CTL1
                bic.b   #UCSWRST, &UCB0CTL1


|__ID__|__Bit__|__Function as set in the code__|
|:-----|:-----|:-----|
|UCCKPH|7|Clock phase select|
|UCMSB|5|Controls direction of Tx/Rx shift registers|
|UCMST|3|Master mode select|
|UCSYNCH|0|Synchronous mode enabled|
|UCSSEL_2|7-6|Select SMCLK as source clock in master mode|
|UCSWRST|0|Software reset enable|

We were given all the code necessarily to see the Nokia display being used.  The next portion of code shown initializes the use of the display by our MSP430.  Using the code, online tec docs, and common sense, we had to fill out a table for decoding the meaning of the symbolic constants.

```
;-------------------------------------------------------------------------------
;   Name:       initNokia       68(rows)x92(columns)
;   Inputs:     none
;   Outputs:    none
;   Purpose:    Reset and initialize the Nokia Display
;-------------------------------------------------------------------------------
initNokia:

    push    R12
    push    R13

    bis.b   #LCD1202_CS_PIN, &P1OUT

    ;-------------------------------------------------------------------------------
    ; Measure the time that the RESET_PIN is held low by the delayNokiaResetLow loop
    bic.b   #LCD1202_RESET_PIN, &P2OUT
    mov #0FFFFh, R12
delayNokiaResetLow:
    dec R12
    jne delayNokiaResetLow
    bis.b   #LCD1202_RESET_PIN, &P2OUT
    ;-------------------------------------------------------------------------------

    mov #0FFFFh, R12
delayNokiaResetHigh:
    dec R12
    jne delayNokiaResetHigh
    bic.b   #LCD1202_CS_PIN, &P1OUT

    ; First write seems to come out a bit garbled - not sure cause
    ; but it can't hurt to write a reset command twice
    mov #NOKIA_CMD, R12
    mov #STE2007_RESET, R13                 ; DECODE HERE
    call    #writeNokiaByte


    mov #NOKIA_CMD, R12
    mov #STE2007_RESET, R13
    call    #writeNokiaByte

    mov #NOKIA_CMD, R12
    mov #STE2007_DISPLAYALLPOINTSOFF, R13           ; DECODE HERE
    call    #writeNokiaByte

    mov #NOKIA_CMD, R12
    mov #STE2007_POWERCONTROL | STE2007_POWERCTRL_ALL_ON, R13   ; DECODE HERE
    call    #writeNokiaByte

    mov #NOKIA_CMD, R12
    mov #STE2007_DISPLAYNORMAL, R13             ; DECODE HERE
    call    #writeNokiaByte

    mov #NOKIA_CMD, R12
    mov #STE2007_DISPLAYON, R13                 ; DECODE HERE
    call    #writeNokiaByte

    pop R13
    pop R12

    ret
```

|__Symbolic Constant__|__Hex__|__Function__|
|:-----|:-----|:-----|
|#STE2007_RESET|E2|Internal rest|
|#STE2007_DISPLAYALLPOINTSOFF|A4|LCD display, normal|
|#STE2007_POWERCONTROL|-|Sets on chip power supply function|
|#STE2007_POWERCTRL_ALL_ON|2F|Booster, voltage regulator, voltage follower; all on|
|#STE2007_DISPLAYNORMAL|A6|LCD display, normal|
|#STE2007_DISPLAYON|AF|Display turned on|


#Code

##Pseudocode

No psudocode was needed since everything was provided.  Only small adjustments were made to the given code in order to meet functionality requirements.  Below is the given code, lab3.asm.

```
;-------------------------------------------------------------------------------
;	Chris Coulston
;	Fall 2014
;	MSP430G2553
;	Draw a new vertical bar on the Nokia 1202 display everytime that SW3
;	is pressed and released.
;-------------------------------------------------------------------------------
	.cdecls C,LIST,"msp430.h"		; BOILERPLATE	Include device header file


LCD1202_SCLK_PIN:				.equ	20h		; P1.5
LCD1202_MOSI_PIN: 				.equ	80h		; P1.7
LCD1202_CS_PIN:					.equ	01h		; P1.0
LCD1202_BACKLIGHT_PIN:			.equ	10h
LCD1202_RESET_PIN:				.equ	01h
NOKIA_CMD:						.equ	00h
NOKIA_DATA:						.equ	01h

STE2007_RESET:					.equ	0xE2
STE2007_DISPLAYALLPOINTSOFF:	.equ	0xA4
STE2007_POWERCONTROL:			.equ	0x28
STE2007_POWERCTRL_ALL_ON:		.equ	0x07
STE2007_DISPLAYNORMAL:			.equ	0xA6
STE2007_DISPLAYON:				.equ	0xAF

 	.text								; BOILERPLATE	Assemble into program memory
	.retain								; BOILERPLATE	Override ELF conditional linking and retain current section
	.retainrefs							; BOILERPLATE	Retain any sections that have references to current section
	.global main						; BOILERPLATE

;-------------------------------------------------------------------------------
;           						main
;	R10		row value of cursor
;	R11		value of @R12
;
;	When calling writeNokiaByte
;	R12		1-bit	Parameter to writeNokiaByte specifying command or data
;	R13		8-bit	data or command
;
;	when calling setAddress
;	R12		row address
;	R13		column address
;-------------------------------------------------------------------------------
main:
	mov.w   #__STACK_END,SP				; Initialize stackpointer
	mov.w   #WDTPW|WDTHOLD, &WDTCTL  	; Stop watchdog timer
	dint								; disable interrupts

	call	#init						; initialize the MSP430
	call	#initNokia					; initialize the Nokia 1206
	call	#clearDisplay				; clear the display and get ready....

	clr		R10							; used to move the cursor around
	clr		R11

while1:
	bit.b	#8, &P2IN					; bit 3 of P1IN set?
	jnz 	while1						; Yes, branch back and wait

while0:
	bit.b	#8, &P2IN					; bit 3 of P1IN clear?
	jz		while0						; Yes, branch back and wait

	mov		#NOKIA_DATA, R12			; For testing just draw an 8 pixel high
	mov		#0xE7, R13					; beam with a 2 pixel hole in the center
	call	#writeNokiaByte
	inc		R10							; since rows are 8 times bigger than columns
	and.w	#0x07, R10					; wrap over the row mod 8
	inc		R11							; just let the columm overflow after 92 buttons
	mov		R10, R12					; increment the row
	mov		R11, R13					; and column of the next beam
	call	#setAddress					; we draw

	jmp		while1

;-------------------------------------------------------------------------------
;	Name:		initNokia		68(rows)x92(columns)
;	Inputs:		none
;	Outputs:	none
;	Purpose:	Reset and initialize the Nokia Display
;
;	Registers:	R12 mainly used as the command specification for writeNokiaByte
;				R13 mainly used as the 8-bit command for writeNokiaByte
;-------------------------------------------------------------------------------
initNokia:
	push	R12
	push	R13

	bis.b	#LCD1202_CS_PIN, &P1OUT

	; This loop creates a nice delay for the reset low pulse
	bic.b	#LCD1202_RESET_PIN, &P2OUT
	mov		#0FFFFh, R12
delayNokiaResetLow:
	dec		R12
	jne		delayNokiaResetLow

	; This loop creates a nice delay for the reset high pulse
	bis.b	#LCD1202_RESET_PIN, &P2OUT
	mov		#0FFFFh, R12
delayNokiaResetHigh:
	dec		R12
	jne		delayNokiaResetHigh
	bic.b	#LCD1202_CS_PIN, &P1OUT

	; First write seems to come out a bit garbled - not sure cause
	; but it can't hurt to write a reset command twice
	mov		#NOKIA_CMD, R12
	mov		#STE2007_RESET, R13
	call	#writeNokiaByte

	mov		#NOKIA_CMD, R12
	mov		#STE2007_RESET, R13
	call	#writeNokiaByte

	mov		#NOKIA_CMD, R12
	mov		#STE2007_DISPLAYALLPOINTSOFF, R13
	call	#writeNokiaByte

	mov		#NOKIA_CMD, R12
	mov		#STE2007_POWERCONTROL | STE2007_POWERCTRL_ALL_ON, R13
	call	#writeNokiaByte

	mov		#NOKIA_CMD, R12
	mov		#STE2007_DISPLAYNORMAL, R13
	call	#writeNokiaByte

	mov		#NOKIA_CMD, R12
	mov		#STE2007_DISPLAYON, R13
	call	#writeNokiaByte

	pop		R13
	pop		R12

	ret

;-------------------------------------------------------------------------------
;	Name:		init
;	Inputs:		none
;	Outputs:	none
;	Purpose:	Setup the MSP430 to operate the Nokia 1202 Display
;-------------------------------------------------------------------------------
init:
	mov.b	#CALBC1_8MHZ, &BCSCTL1				; Setup fast clock
	mov.b	#CALDCO_8MHZ, &DCOCTL

	bis.w	#TASSEL_1 | MC_2, &TACTL
	bic.w	#TAIFG, &TACTL

	mov.b	#LCD1202_CS_PIN|LCD1202_BACKLIGHT_PIN|LCD1202_SCLK_PIN|LCD1202_MOSI_PIN, &P1OUT
	mov.b	#LCD1202_CS_PIN|LCD1202_BACKLIGHT_PIN|LCD1202_SCLK_PIN|LCD1202_MOSI_PIN, &P1DIR
	mov.b	#LCD1202_RESET_PIN, &P2OUT
	mov.b	#LCD1202_RESET_PIN, &P2DIR
	bis.b	#LCD1202_SCLK_PIN|LCD1202_MOSI_PIN, &P1SEL			; Select Secondary peripheral module function
	bis.b	#LCD1202_SCLK_PIN|LCD1202_MOSI_PIN, &P1SEL2			; by setting P1SEL and P1SEL2 = 1

	bis.b	#UCCKPH|UCMSB|UCMST|UCSYNC, &UCB0CTL0				; 3-pin, 8-bit SPI master
	bis.b	#UCSSEL_2, &UCB0CTL1								; SMCLK
	mov.b	#0x01, &UCB0BR0 									; 1:1
	mov.b	#0x00, &UCB0BR1
	bic.b	#UCSWRST, &UCB0CTL1

	; Buttons on the Nokia 1202
	;	S1		P2.1		Right
	;	S2		P2.2		Left
	;	S3		P2.3		Aux
	;	S4		P2.4		Bottom
	;	S5		P2.5		Up
	;
	;	7 6 5 4 3 2 1 0
	;	0 0 1 1 1 1 1 0		0x3E
	bis.b	#0x3E, &P2REN					; Pullup/Pulldown Resistor Enabled on P2.1 - P2.5
	bis.b	#0x3E, &P2OUT					; Assert output to pull-ups pin P2.1 - P2.5
	bic.b	#0x3E, &P2DIR

	ret

;-------------------------------------------------------------------------------
;	Name:		writeNokiaByte
;	Inputs:		R12 selects between (1) Data or (0) Command string
;				R13 the data or command byte
;	Outputs:	none
;	Purpose:	Write a command or data byte to the display using 9-bit format
;-------------------------------------------------------------------------------
writeNokiaByte:

	push	R12
	push	R13

	bic.b	#LCD1202_CS_PIN, &P1OUT							; LCD1202_SELECT
	bic.b	#LCD1202_SCLK_PIN | LCD1202_MOSI_PIN, &P1SEL	; Enable I/O function by clearing
	bic.b	#LCD1202_SCLK_PIN | LCD1202_MOSI_PIN, &P1SEL2	; LCD1202_DISABLE_HARDWARE_SPI;

	bit.b	#01h, R12
	jeq		cmd

	bis.b	#LCD1202_MOSI_PIN, &P1OUT						; LCD1202_MOSI_LO
	jmp		clock

cmd:
	bic.b	#LCD1202_MOSI_PIN, &P1OUT						; LCD1202_MOSI_HIGH

clock:
	bis.b	#LCD1202_SCLK_PIN, &P1OUT						; LCD1202_CLOCK		positive edge
	nop
	bic.b	#LCD1202_SCLK_PIN, &P1OUT						;					negative edge

	bis.b	#LCD1202_SCLK_PIN | LCD1202_MOSI_PIN, &P1SEL	; LCD1202_ENABLE_HARDWARE_SPI;
	bis.b	#LCD1202_SCLK_PIN | LCD1202_MOSI_PIN, &P1SEL2	;

	mov.b	R13, UCB0TXBUF

pollSPI:
	bit.b	#UCBUSY, &UCB0STAT
	jz		pollSPI											; while (UCB0STAT & UCBUSY);

	bis.b	#LCD1202_CS_PIN, &P1OUT							; LCD1202_DESELECT

	pop		R13
	pop		R12

	ret


;-------------------------------------------------------------------------------
;	Name:		clearDisplay
;	Inputs:		none
;	Outputs:	none
;	Purpose:	Writes 0x360 blank 8-bit columns to the Nokia display
;-------------------------------------------------------------------------------
clearDisplay:
	push	R11
	push	R12
	push	R13

	mov.w	#0x00, R12			; set display address to 0,0
	mov.w	#0x00, R13
	call	#setAddress

	mov.w	#0x01, R12			; write a "clear" set of pixels
	mov.w	#0x00, R13			; to every byt on the display

	mov.w	#0x360, R11			; loop counter
clearLoop:
	call	#writeNokiaByte
	dec.w	R11
	jnz		clearLoop

	mov.w	#0x00, R12			; set display address to 0,0
	mov.w	#0x00, R13
	call	#setAddress

	pop		R13
	pop		R12
	pop		R11

	ret

;-------------------------------------------------------------------------------
;	Name:		setAddress
;	Inputs:		R12		row
;				R13		col
;	Outputs:	none
;	Purpose:	Sets the cursor address on the 9 row x 96 column display
;-------------------------------------------------------------------------------
setAddress:
	push	R12
	push	R13

	; Since there are only 9 rows on the 1202, we can select the row in 4-bits
	mov.w	R12, R13			; Write a command, setup call to
	mov.w	#NOKIA_CMD, R12
	and.w	#0x0F, R13			; mask out any weird upper nibble bits and
	bis.w	#0xB0, R13			; mask in "B0" as the prefix for a page address
	call	#writeNokiaByte

	; Since there are only 96 columns on the 1202, we need 2 sets of 4-bits
	mov.w	#NOKIA_CMD, R12
	pop		R13					; make a copy of the column address in R13 from the stack
	push	R13
	rra.w	R13					; shift right 4 bits
	rra.w	R13
	rra.w	R13
	rra.w	R13
	and.w	#0x0F, R13			; mask out upper nibble
	bis.w	#0x10, R13			; 10 is the prefix for a upper column address
	call	#writeNokiaByte

	mov.w	#0x00, R2			; Write a command, setup call to
	pop		R13					; make a copy of the top of the stack
	push	R13
	and.w	#0x0F, R13
	call	#writeNokiaByte

	pop		R13
	pop		R12

	ret


;-------------------------------------------------------------------------------
;           System Initialization
;-------------------------------------------------------------------------------
	.global __STACK_END					; BOILERPLATE
	.sect 	.stack						; BOILERPLATE
	.sect   ".reset"                	; BOILERPLATE		MSP430 RESET Vector
	.short  main						; BOILERPLATE
```



Personally, I would have given up if I had to write that code myself.  It's done very well and breaks it down to show what is happening where.  The given code prints a vertical line to the Nokia display with the middle 2 bits not there (logic 0s).  Every time the button is pressed, the column is shifted right and down before pirnted again.  The below picture was given to show what should be happening.  Thankfully that's what I got so no troubleshooting was needed.


![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab3/master/picture_given.jpg "Given")


##Required Functionality

Required functionality was actually extremely simple.  Not including the label, I added in 3 lines of code to the lab3.asm that was given to us.  The difference compared to the given code can be seen at lines 65 and 68-71 (shown below).  In order to make a solid line instead of a line with the middle missing, I changed "0xE7" to "0XFF" which is logic 1 for all 8 bits.  Lines 68-71 is a simple for loop to run the writeNokiaByte function 8 times.  The result is a 8 by 8 square printed to the Nokia display.


      while1:
      	bit.b	#8, &P2IN					; bit 3 of P1IN set?
      	jnz 	while1						; Yes, branch back and wait

      while0:
      	bit.b	#8, &P2IN					; bit 3 of P1IN clear?
      	jz		while0						; Yes, branch back and wait

      	mov		#NOKIA_DATA, R12			; For testing just draw an 8 pixel high
      	mov		#0xFF, R13					; beam with a 2 pixel hole in the center
      	mov		#0x8, R7

      NathansLoop:
      	call	#writeNokiaByte
      	dec.w	R7
      	jnz NathansLoop

      	inc		R10							; since rows are 8 times bigger than columns
      	and.w	#0x07, R10					; wrap over the row mod 8
      	inc		R11							; just let the columm overflow after 92 buttons
      	mov		R10, R12					; increment the row
      	mov		R11, R13					; and column of the next beam
      	call	#setAddress					; we draw

      	jmp		while1


Below is a picture of my Nokia display running the required functionality program.  I clicked the button a few times to show the 8x8 block moving like it usually would.


![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab3/master/min_func.jpg "Minimum")

##A Functionality
(Soon to come...hopefully)


##Debugging
The requirements were very straight forward so knowing whether or not it worked was simple.  I changed the given code just a little bit in order to print a 8x8 pixel box on the Nokia display.  3 lines of code caused no trouble at all and worked the first try (never thought I'd get to say that sentence.... Ever).  A functionality is just a tad bit trickier.  In order to do A Functionality, you can't just understand the idea behind the code.  I actually need to know which line does what.  I have to be able to understand the setAddress function in order to put in if statements for each button press.  As I'm typing this, I haven't figured it out yet, but hopefully soon.  To get the general idea to make sure I know what I'm doing, I wrote up a text code using C, then using the "disassembly" to see the equivalent code in Assembly Language.  Sort of like pseudocode, it's helping me understand what I'm trying to do.

##Testing Methodology / Results
"I'm definitely one of those people that will code a very small piece of the problem, test it, and if correct, I store it away and don't touch it.  I'll repeat that method until I have all the pieces of the puzzle!"  I said that in my readme from ECE382_Lab2.  I'm still that person, but not needed as much on Lab3.  Lab3 was not about coding my way to success.  The point of Lab3 was to actually understand what was happening with the Nokia display and MSP430 board.  It was easy to see my results on the Nokia display so I would code a little bit, and try it out.  Tweak things until they were right.

##Observations and Conclusions
The tables were actually rough for me to get since I couldn't visualize a lot of it.  The schematic was easy to follow, no problems.  The others took a lot of attention to get done.  Once it got to messing with the LCD, things flew by.  It was extremely easy to understand the code given to us and how it related to the Nokia display.  The only real problems had was once I had to code myself for the A Functionality.  I think the purpose of the lab was achieved since I feel confortable with the Nokia display and how it interacts with my MSP430 board.

#Other Lab Taskers

##Writing Modes
For this exercise, we imported the below image into paint and added in the result of the operation on the left hand side of the equal sign.  So the 3 blocks on the right side of the equal sign is what I did.  The point is to show our understanding of what happens to bits depending on what instruction is being used out of AND, OR, or XOR.


![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab3/master/writing_mode.jpg "Writing Mode")


##Logic Analyzer
I hooked up my MSP430 to the logic analyzer as we were instructed to on the website for Lab3.  I did not take a picture of the set up.  Looking back, I would have to show which probes were hooked up to which pins on the MSP430.  The below picture is to show that the command/data bit is significantly far away from the 8 data bits.


![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab3/master/Press_Release.jpg "Logic Analyzer")

In order to understand  how the writeNokiaByte function was working, we were required to find the 4 calls in the code that generate the packets of data sent to the Nokia.  We found the line number of each as well as the values of R12 and R13 at the time of the call instruction.  The "purpose" column was pretty much the comments from the code given to us for that line.  Below is the filled out table:

|__Line__|__R12__|__R13__|__Purpose__|
|:-----|:-----|:-----|:-----|
|66|0x1|0XE7|Draws a vertical line with 2 spaces in the middle|
|276|0x0|0XB1|Mask out any weird upper nibble bits|
|288|0x0|0X10|Mask out upper nibble, 10 is the prefix for a upper column address|
|294|0x0|0X1|Write a command and setup call to make a copy of the top of the stack|


Next, we had to decode the data bits of each 9-bit waveform by separating out the MSB, which indicates command or data.  We needed to explain how the packet contents correspond to what was drawn on the display.  The table below shows that information.

|__Line__|__Command/Data__|__8-Bit Packet__|
|:-----|:-----|:-----|
|66|Data|11100111|
|276|Command|10110001|
|288|Command|00010000|
|294|Command|00000000|


##Documentation
On 13 October, I was in the lab with C2C Jeremy Gruszka and C2C Sean Bapty.  We were discussing the lab and I got help with the logic analyzer part on hooking it up right and understanding what I was looking at.
