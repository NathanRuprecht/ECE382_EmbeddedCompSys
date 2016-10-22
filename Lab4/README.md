ECE382_Lab4 - C - "Etch-a-Sketch and Pong"
===========

##Lab4 Purpose
Make the transition from programming in assembly language to C.  In this lab, we will use C to create an etch-a-sketch type program that utilizes some subroutines from Lab3.  We are expected to write clean, maintainable, modular code that is committed regularly to Git.

####Basic Functionality:

Modify our assembly drawBlock function to take in 3 values: x-coordinate, y-coordiante, and a color.  Create an etch-a-sketch program using the directional buttons of the LCD booster pack to control the position of the paint brush.  The paint brush will draw 8x8 blocks of pixels.  The user will change the position of the paint brush by pressing the directional buttons.  Each button press will move the cursor 8 pixels in the direction pressed (see table below).  Pressing the auxillary button (SW3) will toggle the mode of the paint brush between filling squares and clearing squard.  This program must be written in C and call many of the subroutines written as part of Lab3, including drawBlock and changePosition.

|__Button__|__Function__|
|:-----|:-----|
|SW5/Up|Move the cursor up 1 block|
|SW4/Down|Move the cursor down 1 block|
|SW2/Left|Move the cursor left 1 block|
|SW1/Right|Move the cursor right 1 block|
|SW3/Aux|Toggle the color of the paint brush|

####B Functionality:
Create a bouncing block!  This block should move across the screen with no more than 8 pixels per jump.  It should bounce off the walls appropriately, similar to Assignment 6.  An adequate delay movement should be added between each block movement.  The starting position and starting x and y velocities should be initialized in the header, or should be randomly generated.


####A Functionality:

Create Pong on the display!  Create a single paddle that will move up and down on one side of the display, controlled by the up and down buttons.  The block will bounce off the paddle like it bounces off the wall.  When the block misses hitting the paddle, the game will end.


####Bonus Functionality:
Each bonus functionality can be achieved in conjunction with either A or B functionality.  These functionalities must be written in assembly and called by C.  Each is worth 5 points.

Circle: Instead of a bouncing block, create a councing circular ball.

Fine movement: Instead of having the ball/paddle move in 8-pixel jumps, have it move in 1-picels jumps.
 
Inverted display: With a push of the SW3 button, invert the display.  Dark pixels will become light pixels and vice verse.  Instead of a bouncing dark ball, you will have a bouncing light ball.
   
   
   
##Preliminary Design

####Prelab

#####Data Types
Our first task was to complete a table using Page 76 of the C Compiler User's Guide.  We were given the 'Size' and 'Signed/Unsigned' columns and had to fill in the information for 'Type', 'Min value', and 'Max value'.  The point being to familuarize ourselves more with C and what data will be like for each bit size and if it is signed or unsigned.  Below is the table with the information filled in: 

|__Size__|__Signed/Unsigned__|__Type__|__Min Value__|__Max Value__|
|:-----|:-----|:-----|:-----|:-----|
|8-bit|Unsigned|Unsigned Char|0|255|
|8-bit|Signed|Signed Char|-128|127|
|16-bit|Unsigned|Unsigned Short|0|65 535|
|16-bit|Signed|Signed Short|-32 768|32 767|
|32-bit|Unsigned|Unsigned Long|0|4 294 967 295|
|32-bit|Signed|Signed Long|-2 147 483 648|2 147 483 648|
|64-bit|Unsigned|Unsigned Long Long|0|18 446 744 073 709 551 615|
|64-bit|Signed|Signed Long Long|-9 223 372 036 854 775 808|9 223 372 036 854 775 807|

The next table was filled out using the Typedef Wikipedia page.  The point was to make sure we know exactly how many bits we are using with each variable since there is a limited amount of space on microcontrollers.  Instead of using basic data typed like char, short, or long, we specify what is being used by showing the bit count and if it is signed or unsigned.  In the below table, everything was given to us except for the 'C Typedef Declaration' column that we looked up.

|__Type__|__Meaning__|__C Typedef Declaration__|
|:-----|:-----|:-----|
|int8|Unsigned 8-bit value|typedef unsigned char int8;|
|sint8|Signed 8-bit value|typedef sign char int8;|
|int16|Unsigned 16-bit value|typedef unsgined short int16;|
|sint16|Signed 16-bit value|typedef signed short int16;|
|int32|Unsigned 32-bit value|typedef unsigned long int32;|
|sint32|Signed 32-bit value|typedef signed long int32;|
|int64|Unsigned 64-bit value|typedef unsigned  long long int64;|
|sint64|Signed 64-bit value|typedef signed long long int64;|


#####Calling/Return Convention

We used the simpleLab4.c program that was given to us to fill in the next table.  Before diving into the assembly language, we wanted to get an idea of what was going on in this program first.  We were told to step through the program in CCS and record the a, b, c, d, e variables in main, just after the call to the function func in line 16.  Below is that completed table.

|__Iteration__|__a__|__b__|__c__|__d__|__e__|
|:-----|:-----|:-----|:-----|:-----|:-----|
|1st|10.5|9.5|8.5|7.5|2.5|
|2nd|17|16|15|14|9|
|3rd|23.5|22.5|21.5|20.5|15.5|
|4th|30|29|28|27|22|
|5th|36.5|35.5|34.5|33.5|28.5|

Next, we were to look at the assembly code generated by the compiler by selecting the View -> Disassembly menu item.  To complete the table below, we had to first find the code for the function 'func' and write down the starting and ending addresses.  Next, we identified which registers are used to pass the input parameters from main to the function and wrote their identities down below.

|__Parameter__|__Value Sought__|
|:-----|:-----|
|Starting address of 'func'|C0BA|
|Ending address of 'func'|C0C6|
|Register holding w|R12|
|Register holding x|R13|
|Register holding y|R14|
|Register holding z|R15|
|Register holding return value|R12|

#####Cross Language Build Constructs
-What is the role of the 'extern' directive in a .c file?
From what I could tell from the wiki site given, the extern is useful so that a variable can be defined somewhere else and still used by the current program.  If a variable is not defined or has more than 1 definition, there's an error, so it just has to be defined somewhere.  Extern is telling the compiler that memory space does not have to be allocated for this variable since it is defined else where.  In this lab, a number of functions is defined in nokia.asm but used in lab4.c.  To use them, lab4.c uses extern to access those functions.


-What is the role of the '.global' directive in an .asm file (used in lines 28-32)?
The .global is how other files can access the functions or variables.  So nokia.asm makes a number of functions global so they can be used by the other files.  The last 2 sentences in the above question kind of goes along with this answer.  Extern and global go hand in hand to be used by a program.


#Code

##Pseudocode

No psudocode was needed since everything was provided.  Only small adjustments were made to the given code in order to meet functionality requirements.

Below is the given code, lab4.c, which dealt with the button presses and calling the functions in the assembly code.

```
#include <msp430g2553.h>

extern void init();
extern void initNokia();
extern void clearDisplay();
extern void drawBlock(unsigned char row, unsigned char col);

#define		TRUE			1
#define		FALSE			0
#define		UP_BUTTON		(P2IN & BIT5)
#define		DOWN_BUTTON		(P2IN & BIT4)
#define		AUX_BUTTON		(P2IN & BIT3)
#define		LEFT_BUTTON		(P2IN & BIT2)
#define		RIGHT_BUTTON	(P2IN & BIT1)


void main() {

	unsigned char	x, y, button_press;

	// === Initialize system ================================================
	IFG1=0; /* clear interrupt flag1 */
	WDTCTL=WDTPW+WDTHOLD; /* stop WD */
	button_press = FALSE;


	init();
	initNokia();
	clearDisplay();
	x=4;		y=4;
	drawBlock(y,x);

	while(1) {

			if (UP_BUTTON == 0) {
				while(UP_BUTTON == 0);
				if (y>=1) y=y-1;
				button_press = TRUE;
			} else if (DOWN_BUTTON == 0) {
				while(DOWN_BUTTON == 0);
				if (y<=6) y=y+1;
				button_press = TRUE;
			} else if (LEFT_BUTTON == 0) {
				while(LEFT_BUTTON == 0);
				if (x>=1) x=x-1;
				button_press = TRUE;
			} else if (RIGHT_BUTTON == 0) {
				while(RIGHT_BUTTON == 0);
				if (x<=10) x=x+1;
				button_press = TRUE;
			}

			if (button_press) {
				button_press = FALSE;
				clearDisplay();
				drawBlock(y,x);
			}
		}
}

```

And below is the other code given to us, nokia.asm, which used assembly language to write the specifics of initializing the display and drawing the block on screen.

```
;-------------------------------------------------------------------------------
;	Chris Coulston
;	Fall 2014
;	MSP430G2553
;	Put some pixels on the Nokia 1202 Diaplsy
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
	.global init
	.global initNokia
	.global clearDisplay
	.global drawBlock


;-------------------------------------------------------------------------------
;	Name:		initNokia		68(rows)x92(columns)
;	Inputs:		none
;	Outputs:	none
;	Purpose:	Reset and initialize the Nokia Display
;-------------------------------------------------------------------------------
initNokia:

	push	R12
	push	R13

	bis.b	#LCD1202_CS_PIN, &P1OUT

	; This loop creates a nice 20mS delay for the reset low pulse
	bic.b	#LCD1202_RESET_PIN, &P2OUT
	mov		#04FFh, R12
delayNokiaResetLow:
	dec		R12
	jne		delayNokiaResetLow
	bis.b	#LCD1202_RESET_PIN, &P2OUT

	; This loop creates a nice 20mS delay for the reset high pulse
	mov		#04FFh, R12
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
	mov.w	#0x00, R12
	and.w	#0x0F, R13			; mask out any weird upper nibble bits and
	bis.w	#0xB0, R13			; mask in "B0" as the prefix for a page address
	call	#writeNokiaByte

	; Since there are only 96 columns on the 1202, we need 2 sets of 4-bits
	mov.w	#0x00, R12
	pop		R13					; make a copy of the column address in R13 from the stack
	push	R13
	rra.w	R13					; shift right 4 bits
	rra.w	R13
	rra.w	R13
	rra.w	R13
	and.w	#0x0F, R13			; mask out upper nibble
	bis.w	#0x10, R13			; 10 is the prefix for a upper column address
	call	#writeNokiaByte

	mov.w	#0x00, R12			; Write a command, setup call to
	pop		R13					; make a copy of the top of the stack
	push	R13
	and.w	#0x0F, R13
	call	#writeNokiaByte

	pop		R13
	pop		R12

	ret


;-------------------------------------------------------------------------------
;	Name:		reDrawDisplay
;	Inputs:		R12 pointer to array[12] containing heights of bars
;	Outputs:	none
;	Purpose:	redraw the entire 96x68 display with 12 vertical bars, the
;				bar in column i has height 8xheight[i] pixels and is 8-bits
;				wide.  The bottom 4 pixels of each bar column are intentionally
;				left blank.  This space could be used for a blinking indicator
;	Registers:	R5	column counter
;				R6	row counter
;-------------------------------------------------------------------------------
reDrawDisplay:
		push	R5				; column counter
		push	R6
		push	R7
		push	R12				; row to draw the block		[0-7
		push	R13				; column to draw the block	[0-11]

		call	#clearDisplay	; start with a blank screen and then overlay bars

		; Since I have set the height array to a bunch of illegal values
		; causing this routine to crash, I thought that it would be a good
		; idea to test the array and saturate any illegal values to safe
		; values bounded by [0-7].
		mov		R12, R5			; R5 is now a tmp pointer to the height array
		mov.w	#11, R6			; loop count var
chkLowerDD:
		mov.b	@R5, R7
		cmp.b	#0,R7
		jhs		chkUpperDD
		mov.b	#0,0(R5)

chkUpperDD:
		cmp.b	#8,R7
		jlo		chkNextDD
		mov.b	#7,0(R5)

chkNextDD:
		inc		R5
		dec		R6
		jge		chkLowerDD

		mov		R12, R5			; copy the height array pointer so that we can call drawBlock
		clr		R13				; Draw columns (indexed by R13 in drawBlock) from left to right on the display

colDD:							; For each column, draw blocks from bottom to top
		clr.w	R6
		mov.b	@R5, R6			; load height[R5] into R6

rowDD:
		mov		R6, R12			; mov the decrementing height into R12
		inv.w	R12				; flip the bits and add one so that the negative can be
		inc.w	R12				; added to 7 to form the complement allowing the
		add.w	#7, R12			; blocks to be draw from bottom to top

		call	#drawBlock		; R12=row, R13=column
		dec		R6				; draw the next lower block - since we allow the
		jge		rowDD			; R6 to equal zero

nextDD:
		; you are now back into the outer loop of colDD
		inc		R5				; point to the next element in heightArray
		inc		R13				; prepare to start drawing the next column
		cmp		#12, R13		; check if we have drawn all 12 columns (if we are done)
		jnz		colDD

		pop		R13
		pop		R12
		pop		R7
		pop		R6
		pop		R5

		ret						; bye-bye


;-------------------------------------------------------------------------------
;	Name:		drawBlock
;	Inputs:		R12 row to draw block
;				R13	column to draw block
;	Outputs:	none
;	Purpose:	draw an 8x8 block of black pixels at screeen cordinates	8*row,8*col
;				The display screen, for the purposes of this routine, is divided
;				into 8x8 blocks.  Consequently the codinate system, for the purposes
;				of this routine, start in the upper left of the screen @ (0,0) and
;				end @ (11,7) in the lower right of the display.
;	Registers:	R5	column counter to draw all 8 pixel columns
;-------------------------------------------------------------------------------
drawBlock:
	push	R5
	push	R12
	push	R13

	rla.w	R13					; the column address needs multiplied
	rla.w	R13					; by 8in order to convert it into a
	rla.w	R13					; pixel address.
	call	#setAddress			; move cursor to upper left corner of block

	mov		#1, R12
	mov		#0xFF, R13
	mov.w	#0x08, R5			; loop all 8 pixel columns
loopdB:
	call	#writeNokiaByte		; draw the pixels
	dec.w	R5
	jnz		loopdB

	pop		R13
	pop		R12
	pop		R5

	ret							; return whence you came

```


##Required Functionality

Required functionately was (thankfully) a piece of cake.  I was able to finish it in class the first day of our lab.  I pretty much just wrote an if statement in nokia.asm that made the square solid (0xFF) or clear (0x0).  And commenting out "clearDisplay()" in lab4.c made it so that the blocks drawn stayed there and then I could "erase" them by bringing the cleared block over them.  Below shows the changes I made to the nokia.asm and lab4.c files described above:

nokia.asm changes:
```
		mov		#1, R12
		cmp		#0x1, R14
		jeq		make_filled
	make_unfilled:
		mov		#0x0, R6
		jmp		next_step
	make_filled:
		mov		#0xFF, R6
	next_step:
		mov		R6, R13
		mov.w	#0x08, R5			; loop all 8 pixel columns
```



lab4.c changes:
```

			} else if (AUX_BUTTON == 0) {
				while(AUX_BUTTON ==0);
				filled = -filled;
				button_press = TRUE;
			}

			if (button_press) {
				button_press = FALSE;
				//clearDisplay();
				drawBlock(y,x, filled);
			}
```


##B Functionality
B functionality took a little longer because it was a change from the user changing the status of the block, to the block moving on its own at the rate of a delay.  All the work was pretty much done in the lab4.c file.  I added in a velocity variable for the x and y direction.  After the code checked if the block hit a wall, the x and y positions were updated using the x and y velocities.  The only difference to nokia.asm is I took out the if statement.  So nokia.asm used in b functionality is the same as the given nokia.asm code.

lab4.c changes:
```
		//Ball bouncing off of ceiling
		if (y<=0) y_vel = -y_vel;

		//Ball bouncing off of floor
		if (y>6) y_vel = -y_vel;

		//Ball bouncing off of left wall
		if (x<=0) x_vel = -x_vel;

		//Ball bouncing off of right wall
		if (x>10) x_vel = -x_vel;

		//Update position of ball
		y += y_vel;
		x += x_vel;

		//Delay so we can actually
		__delay_cycles(2000000);

		//Update display
		clearDisplay();
		drawBlock(y,x);
```

##A Functionality
A functionality didn't take long after b functionality except for one problem (explained in debugging).  For A functionality, I just added a paddle on the left side of the screen that moved with button presses.  Making A functionality work (besides that problem that will be explained) took maybe 10 minutes.  I added a drawPaddle() function (copy, paste, and change name of drawBlock() ) in nokia.asm and the necessary code in lab4.c to move the paddle and check when the block hit the paddle and when it got passed the padle.  Below are the changed I made to each file.

nokia.asm changes:
```
;-------------------------------------------------------------------------------
;	Name:		drawPaddle
;	Inputs:		R12 row to draw block
;				R13	column to draw block
;	Outputs:	none
;	Purpose:	draw an 8x8 block of black pixels at screeen cordinates	8*row,8*col
;				The display screen, for the purposes of this routine, is divided
;				into 8x8 blocks.  Consequently the codinate system, for the purposes
;				of this routine, start in the upper left of the screen @ (0,0) and
;				end @ (11,7) in the lower right of the display.
;	Registers:	R5	column counter to draw all 8 pixel columns
;-------------------------------------------------------------------------------
drawPaddle:
	push	R5
	push	R12
	push	R13

	rla.w	R13					; the column address needs multiplied
	rla.w	R13					; by 8in order to convert it into a
	rla.w	R13					; pixel address.
	call	#setAddress			; move cursor to upper left corner of block

	mov		#1, R12
	mov		#0xFF, R13
	mov.w	#0x02, R5			; loop all 8 pixel columns
loopdC:
	call	#writeNokiaByte		; draw the pixels
	dec.w	R5
	jnz		loopdC

	pop		R13
	pop		R12
	pop		R5

	ret							; return whence you came
```

lab4.c changes:
```
void main() {

	unsigned char	x, y, y_vel, x_vel, x2, y2, x3, y3, cont;
	unsigned long i;

	// === Initialize system ================================================
	IFG1=0; /* clear interrupt flag1 */
	WDTCTL=WDTPW+WDTHOLD; /* stop WD */

	init();
	initNokia();
	clearDisplay();
	x=4;		y=4;	x_vel=1;	y_vel=1;
	x2=1;		y2=1;
	x3=x2;		y3=y2-1;
	cont=1;
	drawBlock(y,x);
	drawPaddle(y2, x2);

	while(cont==1) {

		//Ball bouncing off of paddle
		if(y==y2 || y==y3) {
			if(x==x2) {
				x_vel = -x_vel;
			}
		}

		//Ball bouncing off of ceiling
		if (y<=0) y_vel = -y_vel;

		//Ball bouncing off of floor
		if (y>6) y_vel = -y_vel;

		//Ball bouncing off of right wall
		if (x>10) x_vel = -x_vel;

		//End game
		if (x<=0) cont=0;

		//Update position of ball
		y += y_vel;
		x += x_vel;

		//Paddle
		for(i=0;i<100000;i++) {
			while (UP_BUTTON == 0) {
				while(UP_BUTTON == 0);
				if (y2>=2) y2=y2-1;//lower half of paddle
				if (y3>=1) y3=y3-1;//upper half of paddle
			}
			while (DOWN_BUTTON == 0) {
				while(DOWN_BUTTON == 0);
				if (y2<=6) y2=y2+1;//lower half of paddle
				if (y3<=5) y3=y3+1;//upper half of paddle
			}
		}



		clearDisplay();
		drawBlock(y,x);
		drawPaddle(y2, x2);
		drawPaddle(y3, x3);

		}
}
```

##Debugging
Capt Trimble likes to see me suffer....  So during A functionality, I could not figure out why my code was breaking when I tried to run it.  The block would be drawn along with half of the paddle.  I made a for loop to act as the delay so I could check if the button was pressed to move the paddle.  Turns out that I made my i value too big for what it was initialized as, an unsigned char.  Capt Trimble made sure I watched my variables as I stepped through the for loop.  I watched the i value go from 0x0 to 0xFF.  After I clicked it again, i went back to 0x0.  My lesson learned is that my program was stuck in the for loop because the condition was never met.  I changed my i to an unsigned long and the condition for i < 100000.  After that, A functionality worked like a beauty.  10 minutes for A functionality turned into ~50 minutes.  But I learned my lesson.

##Testing Methodology / Results
There was almost no testing.  I knew what I needed to code and got it on the first try for every functionality (except A... thank you again Capt Trimble).  I would still code a bit and run the program to make sure I didn't break it.  But everything went swimmingly.

##Observations and Conclusions
Prelab was easy!  We should have more like those so I can just look up the table and write in the values.  My main lesson learned making sure the values I assign to variables match up to how big I define them as.  I got used to making everything a char so the i value caused a lot of pain.

##Documentation
Other than talking to our instructors (Dr. Coulston and Capt Trimble) I received no outside help on this lab.

