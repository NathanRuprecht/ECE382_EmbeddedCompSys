//-----------------------------------------------------------------
// Name:	C2C Nathan Ruprecht
// File:	lab6.c
// Date:	Fall 2014
// Purp:	Required functionality: robot goes forward, backward, a
//			small left turn, small right turn, big left turn, and a
//			big right turn
//-----------------------------------------------------------------
#include <msp430.h>

void forward();
void backward();
void smallLeft();
void smallRight();
void bigLeft();
void bigRight();
void pause();

//-----------------------------------------------------------------
//-----------------------------------------------------------------
void main(void)
{
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P1DIR &= ~BIT3;
    P1REN |= BIT3;

    //directional control
    P2DIR |= BIT1;                          // P2.1 is left directional control
    P2DIR |= BIT3;                          //P2.3 is right directional control

    //left motor
    P2DIR |= BIT2;                          // P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;                          // P2.2 is associated with TA1CCTL1
    P2DIR |= BIT0;							//enables left motor
    P2OUT &= ~BIT0;							//disables left motor

    //right motor
    P2DIR |= BIT4;                          // P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;                          // P2.4 is associated with TA1CCTL2
    P2DIR |= BIT5;							//enables right motor
    P2OUT &= ~BIT5;							//disables right motor

    TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

    TA1CCR1 = 0x0020;

    TA1CCR2 = 0x0020;

    while (1)
    {
    	forward();
    	pause();
    	backward();
    	pause();
    	smallLeft();
    	pause();
    	smallRight();
    	pause();
    	bigLeft();
    	pause();
    	bigRight();
    	pause();
    } // end loop
} // end main

void forward(void)
{
	//left motor
	P2OUT |= BIT0;						//enable left motor
	P2OUT |= BIT1;                     //move left wheel forward
	TA1CCTL1 = OUTMOD_7;

	//right motor
	P2OUT |= BIT5;						//enable right motor
	P2OUT &= ~BIT3;                     //move right wheel forward
	TA1CCTL2 = OUTMOD_3;

	int i,k;
	for(i = 0; i < 20000; i++)
	{
		for(k = 0; k < 10; k++);
	}
}

void backwards(void)
{
	//left motor
	P2OUT |= BIT0;						//enable left motor
	P2OUT &= ~BIT1;                     //move left wheel backwards
	TA1CCTL1 = OUTMOD_3;

	//right motor
	P2OUT |= BIT5;						//enable right motor
	P2OUT |= BIT3;                     //move right wheel backwards
	TA1CCTL2 = OUTMOD_7;

	int i,k;
	for(i = 0; i < 20000; i++)
	{
		for(k = 0; k < 10; k++);
	}
}

void smallLeft(void)
{
	//left motor
	P2OUT |= BIT0;						//enable left motor
	P2OUT &= ~BIT1;                     //move left wheel backwards
	TA1CCTL1 = OUTMOD_3;

	//right motor
	P2OUT |= BIT5;						//enable right motor
	P2OUT &= ~BIT3;                     //move right wheel forward
	TA1CCTL2 = OUTMOD_3;

	int i,k;
	for(i = 0; i < 3650; i++)
	{
		for(k = 0; k < 10; k++);
	}
}

void bigLeft(void)
{
	//left motor
	P2OUT |= BIT0;						//enable left motor
	P2OUT &= ~BIT1;                     //move left wheel backwards
	TA1CCTL1 = OUTMOD_3;

	//right motor
	P2OUT |= BIT5;						//enable right motor
	P2OUT &= ~BIT3;                     //move right wheel forward
	TA1CCTL2 = OUTMOD_3;

	int i,k;
	for(i = 0; i < 6150; i++)
	{
		for(k = 0; k < 10; k++);
	}
}

void smallRight(void)
{
	//left motor
	P2OUT |= BIT0;						//enable left motor
	P2OUT |= BIT1;                     //move left wheel forward
	TA1CCTL1 = OUTMOD_7;

	//right motor
	P2OUT |= BIT5;						//enable right motor
	P2OUT |= BIT3;                     //move right wheel backwards
	TA1CCTL2 = OUTMOD_7;

	int i,k;
	for(i = 0; i < 3500; i++)
	{
		for(k = 0; k < 10; k++);
	}
}

void bigRight(void)
{
	//left motor
	P2OUT |= BIT0;						//enable left motor
	P2OUT |= BIT1;                     //move left wheel forward
	TA1CCTL1 = OUTMOD_7;

	//right motor
	P2OUT |= BIT5;						//enable right motor
	P2OUT |= BIT3;                     //move right wheel backwards
	TA1CCTL2 = OUTMOD_7;

	int i,k;
	for(i = 0; i < 6000; i++)
	{
		for(k = 0; k < 10; k++);
	}
}

void pause(void)
{
	//disable both motors
	P2OUT &= ~BIT0;
	P2OUT &= ~BIT5;

	int i,k;
	for(i = 0; i < 9000; i++)
		{
			for(k = 0; k < 100; k++);
		}
}

