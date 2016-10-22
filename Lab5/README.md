ECE382_Lab5 - Interrupts - "Remote Control Decoding"
====================================================

####Functionality video
https://www.youtube.com/watch?v=U5ATsBlcOGk&list=UUlRCgLUgi5g9Fu_dqypjz5A

##Lab5 Purpose
In this lab, we used our knowledge of interrupts and the Timer_A subsystem to reverse engineer a remote control.  We needed to understand and be able to use the timer interrupt and the general purpose pin interrupt to decode the remote control.

###Checkpoints:

####Day 1: 

Learn the timing and bit patterns for your remote control.

The point of day 1 was to see our code running in CCS as well as on the function generator and putting 2 and 2 together to understand what was happening.  In CCS, there were 2 variables called timer1 and timer2 that showed an array of numbers.  On the function generator, I got the following picture:

![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab5/master/funcgen.jpg "Function Generator")

The timer0 array numbers went with the logic 0 half-pulses (the lines on the bottom) while timer1 matched with the logic 1 half-pulse (top lines).  To dumb it down to my terms, a bottom line was paired up with a top line to make a single data point.  Depending on the lenth of the top line determine whether the data was a 1 or 0.  The value in the array on CCS matched up with the function by showing time.  To prove this true, I meausered a number of the data points on the function generator and compared them to the values in CCS.  The results are in the table below:


|__Pulse__|__Duration(ms)__|__Timer A Counts__|
|:-----|:-----|:-----|
|Start logic 0 half-pulse|9|8908|
|Start logic 1 half-pulse|4.46|4439|
|Data 1 logic 0 half-pulse|0.6|591|
|Data 1 logic 1 half-pulse|1.64|1622|
|Data 0 logic 0 half-pulse|0.6|592|
|Data 0 logic 1 half-pulse|0.54|527|
|Stop logic 0 half-pulse|0.58|570|
|Stop logic 1 half-pulse|40|39399|

So the variables in CCS match up with the time length of the data on the function generator.  The entire packet on the function generator is pretty much the 1s and 0s for the button pressed on the remote.  So each button has a different code depending on its function.  To show that, the below table was also filled out:

|__Button__|__Code (not including start and stop bits)__|__Code in Hex__|
|:-----|:-----|:-----|
|0|10011110011000010000100011110111|9E6108F7|
|1|10011110011000011000100001110111|9E618877|
|2|10011110011000010100100010110111|9E6148B7|
|3|10011110011000011100100000110111|9E61C837|
|Open/Close|10011110011000011000000001111111|9E61807F|
|SKIP >>|10011110011000011110000000011111|9E61E01F|
|<< SKIP|10011110011000010010000011011111|9E6120DF|
|SEARCH >>|10011110011000010110000010011111|9E61609F|
|<< SEARCH|10011110011000011010000001011111|9E61A05F|

Usually the table would have the buttons labeled as 0, 1, 2, 3, Power, VOL +, VOL -, CH +, CH -.  But my remote seemed to go to an audio system instead of a TV so my table shows different buttons that my classmates might have.


####Day 2:

Demonstrate that our code can receive and decode button presses from the remote control


####Required Functionality:

Using both timer interrupts and a port 2 interrup, turn an LEC on and off with one button on the remote and turn another LED on and off with a different button.

Day 2 / Required Functionality was probably more trouble than it should have been.  I struggled hard just to wrap my head around what was going on.  The department was nice enough to give us the below picture:

![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab5/master/picture4code.jpg "How to Code")

No joke, all the information we needed was in the above picture.  All we had to do is make it C code in CCS and add in our button press information in the header file and in main().  I struggled to even understand what Port 2 Vector was doing, let alone trying to code it.  And in Timer0_A1, I just coded it and let it be.  Most of this, I didn't question.  If I coded it correctly, I thought "cool," and moved on.


####A Functionality:

Use the buttons on a remote control to either control your lab #4 etch-a-sketch (up, down, right, left, color) or your pong game.

I'm sorry.  Didn't even attempt it.  I was having enough problems just getting the required functionality.

   
##Preliminary Design

####Prelab

Read the assignment so we can make good use of our time in class.


#Code

##Pseudocode

No psudocode was needed since almost everything was provided.  Only small adjustments were made to the given code in order to meet functionality requirements.

First thing I did was take the information for my buttons from Day 1 and put their hex values into the header file:


```
#define	O_C	0x9E61807F // Open/Close on Remote #6
#define	ONE	0x9E618877
#define	TWO	0x9E6148B7
#define	THR	0x9E61C837

#define	SKIP_RT	0x9E61E01F // Skip >>
#define	SKIP_LT	0x9E6120DF // << Skip
#define	SRCH_RT	0x9E61609F // Search >>
#define	SRCH_LT	0x9E61A05F // << Search
```


For required functionality, I only used 4 of the buttons on the remote: Skip forward, skip back, search forward, and search back.  Again, using a remote to an audio system meant my buttons would be different from my classmates, but since we all were required to turn an led on and off, it really didn't matter which buttons were used as long as we were consistent.  The header file might have been the easiest part.  The majority of my time was spent on the start5.c code where everything came together.

	void main(void) {
	
		initMSP430(); // Setup MSP to process IR and buttons
		
		while(1) {

			//wait for a new IrPacket
			while (newIrPacket == FALSE);

			//Turn on/off the green led
			if (IrPacket == SKIP_RT) P1OUT |= BIT6;
			if (IrPacket == SKIP_LT) P1OUT &= BIT6;

			//Turn on/off the red led
			if (IrPacket == SRCH_RT) P1OUT |= BIT0;
			if (IrPacket == SRCH_LT) P1OUT &= BIT0;

		} // end infinite loop
	} // end main

Main() was the easiest part of this file for me.  Possibly because I actually understood what needed to happen.  And the fact that it was really just a couple of if statements.  The idea is to wait until any button was pressed.  If it was the skip buttons, I toggled the greed led.  If it was one of the search buttons, I toggled the red led.  Simple as that!

For the rest of start5.c, I know joke followed the picture given to us (thank you so much, I would've died without it).  Defining constants and global variables were no problem.  My main issue was when I got to port2_vector.



	#pragma vector = PORT2_VECTOR // This is from the MSP430G2553.h file
	__interrupt void pinChange (void) {

		int8 pin;
		int16 pulseDuration; // The timer is 16-bits

		if (IR_PIN) pin=1; else pin=0;

		switch (pin) { // read the current pin level
			case 0: //v Edge // !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
				pulseDuration = TAR;

				//Logic 0
				if( (pulseDuration > minLong0Pulse) && (pulseDuration < maxLogic0Pulse)) {
					irPacket = (irPacket << 1) | 0;
				}

				//Logic 1
				if( (pulseDuration > minLogic1Pulse) && (pulseDuration < maxLogic1Pulse)) {
					irPacket = (irPacket << 1) | 1;
				}

				packetData[packetIndex++] = pulseDuration;
				TACTL = 0; //Turn off TmrA
				LOW_2_HIGH; // Setup pin interrupr on positive edge
				break;

			case 1: //^ Edge // !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
				TAR = 0x0000; // time measurements are based at time 0
				TACTL = MC_1; //Turn on TmrA
				TACTL ^= ~TAIE; //enable TmrA interrupt
				HIGH_2_LOW; // Setup pin interrupr on positive edge
				break;

		} // end switch

		P2IFG &= ~BIT6; // Clear the interrupt flag to prevent immediate ISR re-entry
	} // end pinChange ISR



Looking at it now, it really wasn't that bad.  But man I struggled looking at the picture and coding what needed to be coded.  I put the comments next to each case just so I knew exactly what box in the picture went to which case in the program.  All case 1 is doing is writing a 1 or 0 as the new ir packet is coming in.  Case 1 is just turning on and enabling the interrupt.



	#pragma vector = TIMER0_A1_VECTOR // This is from the MSP430G2553.h file
	__interrupt void timerOverflow (void) {

		TACTL = MC_0; //Turn off TmrA
		TACTL &= ~TAIE; //Turn off TmrA interrupt
		PACKETiNDEX = 0; //Clear packetIndex
	
		//set newPacket flag
		
		TACTL &= ~TAIFG; //Clear TAIFG

	}


The timer0_a1 was fairly straight forward.  This was the first part of the picture I tried to tackle so Dr. Coulston was there to walk me through it.  This was at the beginning of Day 2 so he helped me understand what the picture was saying and the syntax of how to code it.


##Required Functionality

The main thing I had to change in the given files to get required functionality to work was the header file for my buttons, then main() in start5.c.  Boom.  Winner.  Eventually.


##Debugging
A lot of the debugging happened in class.  Day 1, it took a little while to get going and understanding what I was supposed to be seeing.  Dr. Coulston was able to give pointers here and there to the class to point us in the right direction.  I was able to finish all of Day 1 activities in class.  Day 2 was rough.  Like I said earlier, Dr. Coulston walked me through a portion of the code so I could get an idea of what the heck I was supposed to do.  I also got Capt Trimble to explain some portions of the code and what it did for the big picture.

##Testing Methodology / Results
I had to come into the lab on my own time quite a bit.  If there were people there, I usually ended up working on other homework.  My thought was that I wanted to be the only one "zapping" my sensor so I could get accurate reading on what was happening.  But really, there weren't any problems with my code.  Everything went pretty smooth once I finished coding the picture.  There were multiple times that the cadets bounced ideas off of each other trying to figure it out.  I took pieces from here and there to get to where I am (sorry but I really don't have specific instances for a Documentation Statement...).

##Observations and Conclusions
Again, I probably took longer than most figuring this out.  A lot of my problems came from just not knowing how each piece fit the puzzle.  Once it was all there, big picture seemed to come together.

##Documentation
Like I said in "Testing Methodology / Results" there were times when the students tried to help each other out with ideas.

Other than talking to our instructors (Dr. Coulston and Capt Trimble) I received no outside help on this lab.
