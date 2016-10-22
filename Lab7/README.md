ECE382_Lab7 - A / D CONVERSION - "Robot Sensing"
================================================

####Functionality Video
https://www.youtube.com/watch?v=w16iDwAkCj4&list=UUlRCgLUgi5g9Fu_dqypjz5A

###Background Info:
The lab is designed to assist us in learning the concepts associated with the analog-to-digital converter (ADC) for the MSP430.  A set of 3 infrared (IR) emitter and detector pairs on the robot is used to bring in analog voltage values for the ADC.  We are to program the MSP540 to interpret these incoming voltages to determine whether the robot is approaching a wall in front or on one of its sides.  The skills we will learn from this lab will come in handy in the future, especially for the senior design project since many designs require us to interface analog systems with digital systems.

By using the headers available to us on the top of the robot PCB, we can read a value between 0V and 5V that is proportional to the distance to an object in front of the IR sensor.  The left/right directions are relative to a person standing behind the robot.

####Required Functionality:
Use the ADC subsystem to light LEDs based on the presence of a wall.  The presence of a wall next to the left sensor should light LED1 on the launchpad board.  The presence of a wall next to the right sensor should light LED2 on the launchpad board.  Demonstrate that the LEDs do not light until they come into close proximity with the wall.

####B Functionality:
Create a standalone library for your ATD code and release it on Github.  This should be separate from your lab code.  It should have a thoughtful interface and README, capable of being reused in the robot maze laboratory.

####A Functionality:
Since each robot's sensors are a little bit different, we ened to fully characterize the sensor for the robot.  Create a table and graphical plot that shows the ATD values for a variety of distances from a maze wall.  This table/graph must be generated for each IR sensor.  Use these values to determine how the IR sensors work so you can properly use them to solve the maze.

##Preliminary Design
####Prelab
  First I need to make sure the sensors are working.  I'll apply 5V to the sensors and use the camera on my phone to see that they sensors are showing up as purple.  Purple means they're on and working!  We learned a bit last class about making a proportion of the voltage level and a numerical value.  For this lab, once a voltage reads to a certain value, that will trigger the LED to come on to show that there is an obstacle there.  Honestly, I'll have to get EI on libraries because I really do not know how to go about that portion of the lab.

#Code
##Given Code
The below code was given to us in order to start us out in the right direction.  This code reads one of the sensors and toggles a LED.  Our job was to correctly wire our robot so that we can use the sensors depending on the ports we use.  Turning a LED on or off would be one of the conditions so that wouldn't be as hard.

```
//-----------------------------------------------------------------
// Name:	Coulston
// File:	lab7.c
// Date:	Fall 2014
// Purp:	Collect analog samples from P1.3 and P1.4
//-----------------------------------------------------------------
#include "msp430g2553.h"

void initMSP430();

//----------------------------------------------------------------------
//----------------------------------------------------------------------
int main(void) {

	unsigned short sample[16];									// Just to analyze the values
	unsigned char i = 0;										// index into sample array

	IFG1=0; 													// clear interrupt flag1
	WDTCTL = WDTPW + WDTHOLD;									// disable WDT

	BCSCTL1 = CALBC1_8MHZ;										// 8MHz clock
	DCOCTL = CALDCO_8MHZ;

	P1DIR = BIT0;												// Set the red LED as output
```
	
	The first portion of the code initializes everything.  It's really to just make the variables, set the clock, and make the LED ports an output.  For functionality, I only made minor changes and it was to make more variables.
	
```	
	while(1) {

		  // Configure P1.4 to be the ADC input
		ADC10CTL0 = 0;											// Turn off ADC subsystem
		ADC10CTL1 = INCH_4 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
		ADC10AE0 = BIT4;		 								// Make P1.4 analog input
		ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

		ADC10CTL0 |= ADC10SC;									// Start a conversion
		while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
		sample[i] = ADC10MEM;									// collect that 10-bit value
		if (sample[i] > 0x0200)	P1OUT |= BIT0;
		else					P1OUT &= ~BIT0;

		i = (i+1) & 0xF;							// This is a mod 16 increment
	} // end infinite loop

} // end main
```

Inside the infinite loop is where we are constantly taking in readings from the sensor to see if there's an object close enough to toggle the LED.  In the given code, if the sensor read a value of 0x200 or greater, the LED was turned on, otherwise the LED was off.

##Required Functionality

I started out by just copy and pasting what was given inside the while loop.  I changed the port that was being read on the two lines that has ADC10CTL1 and ADC10AE0.  I made 1.5 be the right IR sensor, 1.4 the center, and 1.3 the left.

```
	while(1) {

		// Right IR Sensor
		ADC10CTL0 = 0;											// Turn off ADC subsystem
		ADC10CTL1 = INCH_5 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
		ADC10AE0 = BIT5;		 								// Make P1.4 analog input
		ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

		ADC10CTL0 |= ADC10SC;									// Start a conversion
		while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
		sample5[i] = ADC10MEM;									// collect that 10-bit value
		if (sample5[i] > 375)	P1OUT |= BIT0;
		else					P1OUT &= ~BIT0;


		// Left IR Sensor
		ADC10CTL0 = 0;											// Turn off ADC subsystem
		ADC10CTL1 = INCH_3 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
		ADC10AE0 = BIT3;		 								// Make P1.4 analog input
		ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

		ADC10CTL0 |= ADC10SC;									// Start a conversion
		while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
		sample3[i] = ADC10MEM;									// collect that 10-bit value
		if (sample3[i] > 400)	P1OUT |= BIT6;
		else					P1OUT &= ~BIT6;

		// Center IR Sensor
		ADC10CTL0 = 0;											// Turn off ADC subsystem
		ADC10CTL1 = INCH_4 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
		ADC10AE0 = BIT4;		 								// Make P1.4 analog input
		ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

		ADC10CTL0 |= ADC10SC;									// Start a conversion
		while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
		sample4[i] = ADC10MEM;									// collect that 10-bit value
		if (sample4[i] > 410)	P1OUT |= BIT6;
		else					P1OUT &= ~BIT6;

		i = (i+1) & 0xF;							// This is a mod 16 increment
	} // end infinite loop
```

The point at which the LED comes on is different for each sensor.  While running the code, I looked at the values of the variables depending on how far my hand was in front of the sensor.  I picked a reasonable distance from each sensor and used the average value as that cutoff value for the code.  This ended up being huge in lab8 where I needed to stop before colliding with a wall.

##A Functionality
Did not attempt...

##Debugging
The lab went well without too much hastle.  A problem I ran into early on is when I literally copy and pasted the given code twice so I had 3 sensor readings.  I copy/pasted the mod 16 increment so i was changing a lot and messing with what I wanted to happen and what the robot was coded to do.

##Testing Methodology / Results
Luckily, not a whole lot of troubleshooting had to happen.  Since I got EI before lab6, Dr. Coulston hinted to me and set me up to be ready to assign certain ports to each component of the lab.

##Observations and Conclusions
I learned pretty quick that the sensors are real sensitive.  My sensors read different values than someone else.  The value could depend on the brightness in the room.  Or how fresh the battery was powering the robot.  We had to be careful with how precise we wanted to be in the cutoff values.

##Documentation
No outside help was received.
