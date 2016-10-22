ECE382_Lab6 - PWM - "Robot Motion"
==================================

####Functionality Video
https://www.youtube.com/watch?v=7_-kEVmyQk0&list=UUlRCgLUgi5g9Fu_dqypjz5A

##Lab6 Purpose
The purpose of this lab is to provide us with experience using the pulse-width modulation features of the MSP430.  We will program the MSP430 to generate pulse-width-modulated waveforms to control the spee / direction of our robot's motors.  In this lab, we will make our robot move forward, backwards, a small turn to the left and right, and a large turn to the left and right.

###Background Info:

Our mobile robots have DC motors to drive the wheel.  The amount of torque provided by the motor is directly proportional to the amount of voltage provided.  Therefor, there are two ways of varying the speed of the DC motors: (1) Provide an analog voltage where the magnitude is proportional to the output torque desired and (2) provide a PWM signal where the duty cycle provides an "average" voltage proportional to the output torque desired.

![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab6/master/pwm_to_motor.png "PWM to Motor")

Above shows that the PWM signal creates a certain duty cycle which will provide an "average" voltage to the motor.  This average voltage is proportional to the motor's output torque.

The motor can move in two directions.  If we ground one terminal of the motor and connect the PWM signal to the other side of the terminal, then the motor shaft moves in one direction.  If we swap the terminals, the motor will move in the opposite direction.  We were told that we might want to program our robot so it turns like a tank; one wheel moves forward while the other one reverses.  We will have to experiment with our robit to find out how long the PWM signal needs to be provided to turn an appropriate amount.

####Required Functionality:

Demonstrate movement forward, backward, a small ( <45 degree ) turn left and right, and a large ( >45 degree ) turn left and right.  The robot should perform these movements sequentially, completely disconnected from a computer ( no USB cord ).

####A Functionality:

Control our robot with our remote control!  We should sue at least four different buttons on the remote: one each to indicate motion directions of forward, backward, left, and right.
   
##Preliminary Design

####Prelab

**Include whatever information from this lab we think will be useful in creating our program.**

Consider the hardware (timer subsystems, chip pinout, etc) and how we will use it to achieve robot control.  Which pins will output which signals we need?  Which side of the motor will we attach these signals to?  How will you use these signals to achieve forward / back / left / right movement?  Also consider how we will setup the PWM subsystem to achieve this control.  What are the registers we'll need to use?  Which bits in those registers are important?  What's the initialization sequence we'll need?  What additional hardware we'll need (regulator, motor driver chip, decoupling capacitor, etc.) and how we'll configure / connect it.  Also consider the interface we'll want to create to our motors.  Do we want to move each motor individually ( moveLeftMotorForward() ) or do we want to move them together ( moveRobotForward() )?

When I first tried the pre lab and coming up with a schematic, I ended up consulting Dr. Google to get an idea of making an h-bridge with the parts available to us.  It involved using 4 NMOS transistors in parallel with 4 zener diodes each.  That set up was for one motor.  Then I found out about the SN754410.  Life became a whole lot easier. 

![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab6/master/PreLabSketch.jpg "PreLab")

EI with Dr. Coulston cleared up sooooo much.  I went in with an h-bridge schematic that I made instead of the chip.  Went in confused and feeling like I know nothing.  The above picture is the notes taken during that hour-ish time frame.  I had the basic idea down of a buffer on each side of the motor, but not how the SN754410 hooked up to it.  We went step by step of how the signal would move through the circuit until I eventually had the entire diagram labled with pin numbers.  The numbers in black ink go to the SN754410 while red ink meant the MSP430.

![alt text](https://raw.githubusercontent.com/NathanRuprecht/ECE382_Lab6/master/schematic.png "Schematic")

Above is my attempt at the very basic idea of a schematic.  I took the notes from EI and applied it to the parts we have.  To make it easier to understand and hook up, I color coted what I could.  In class, I hooked up my robot to show this diagram.  Now, I'll make some adjustments as I find more efficient ways to get what I want.  We also were told about using capacitors in order to get rid of the noise from the motors so I'll have to put those in.


#Code

##Given Code
#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P1DIR &= ~BIT3;
    P1REN |= BIT3;

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.2 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.2 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

    TA1CCR1 = 0x0020;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 0x0020;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode

    while (1) {

    	while((P1IN & BIT3) != 0);			// Wait for a button press
    	while((P1IN & BIT3) == 0);			// and release

        TA1CCR1 = (TA1CCR1 + 0x010) & 0xFF;	// increase duty cycle
        TA1CCR2 = (TA1CCR2 + 0x010) & 0xFF;	// decrease duty cycle

    } // end loop
} // end main

The above code is what was given.  I'm pretty sure we're into the black magic part of ECE.  Dr. Coulston and Capt Trimble are the master magicians that are teaching us young wizards.

##Required Functionality
In order to control the direction of the motors, we had to change the outmod and Md.  Concept wise, I had that down.  I understood what needed to happen.  The syntax was the black magic portion that I just went with.

I started the code with the button press that did nothing.  So before the rest of the program would execute, I had time to put everything down and press the button before the robot moved.

```
#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P1DIR &= ~BIT3;
    P1REN |= BIT3;

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

	while((P1IN & BIT3) != 0);			// Wait for a button press
	while((P1IN & BIT3) == 0);			// and release


    while (1) {

        //TA1CCR1 = (TA1CCR1 + 0x010) & 0xFF;	// increase duty cycle
        //TA1CCR2 = (TA1CCR2 + 0x010) & 0xFF;	// decrease duty cycle


    	// Go Forward
    		TA1CCR1 = 0x00C0;
    		TA1CCTL1 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
    		P2OUT |= BIT1;

    		TA1CCR2 = 0x00C0;
    		TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
    		P2OUT |= BIT3;

    		_delay_cycles(750000);



    		// Go Backwards
            TA1CCR1 = 0x00C0;
            TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode
            P2DIR |= (BIT1|BIT3);
            P2OUT &= ~(BIT1|BIT3);

            TA1CCR2 = 0x00C0;
            TA1CCTL2 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

            _delay_cycles(750000);


            // Small Left
            TA1CCR1 = 0x00C0;
            TA1CCTL1 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
            P2OUT |= BIT1;

            TA1CCR2 = 0x00C0;
            TA1CCTL2 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode
            P2OUT &= ~BIT3;

            _delay_cycles(100000);


            // Small Right
            TA1CCR1 = 0x00C0;
            TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode
            P2OUT &= ~BIT1;

            TA1CCR2 = 0x00C0;
            TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
            P2OUT |= BIT3;

            _delay_cycles(100000);


            // Big left
            TA1CCR1 = 0x00C0;
            TA1CCTL1 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
            P2OUT |= BIT1;

            TA1CCR2 = 0x00C0;
            TA1CCTL2 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode
            P2OUT &= ~BIT3;

            _delay_cycles(500000);


            // Big right
            TA1CCR1 = 0x00C0;
            TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode
            P2OUT &= ~BIT1;

            TA1CCR2 = 0x00C0;
            TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
            P2OUT |= BIT3;

            _delay_cycles(500000);


            // Wait for button press to do it all again!!!
        	while((P1IN & BIT3) != 0);			// Wait for a button press
        	while((P1IN & BIT3) == 0);			// and release

    } // end while loop

} // end main

```
I just changed the direction of each wheel and the delay for each portion... That's it.  Copy and paste is my best friend when coding.

##A Functionality
Didn't even attempt...  I'm not a master wizard


##Debugging
Unfortunately, not a whole lot of debugging hapened.  It either worked or it didn't work.  I did have a problem at first toggling the bit that was not the PWN signal.  Again, I just didn't understand the syntax of the code needed.  In the charlie foxtrot that was Monday, 24 November, I was told the correct way to code so that the other signal going into the motor was what I wanted.


##Testing Methodology / Results
It either worked or it didn't.  For me, it didn't work a lot.  I got it just as people were leaving on the day it was due.


##Observations and Conclusions
I conclude that I need to better my skills in the arts of black magic that is ECE...


##Documentation
There wasn't any specific time to document.  All the in class time I got help.  That Monday afternoon when we were all running around like chickens with their heads cut off.  This was a rough time as far as understanding what we were doing...
