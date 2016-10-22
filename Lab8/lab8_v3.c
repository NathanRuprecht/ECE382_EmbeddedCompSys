//-----------------------------------------------------------------
// Name:	C2C Nathan Ruprecht
// File:	lab8.c
// Date:	Fall 2014
// Purp:	Get through the maze
//-----------------------------------------------------------------
#include "msp430g2553.h"

void initMSP430();
void req_func();
void b_func();
void a_func();
void go_forward();
void turn_left();
void turn_right();
void pause();
int check_sensors();
void initSensors();
void initMotors();

#define green_on	P1OUT |= BIT6
#define green_off	P1OUT &= ~BIT6
#define red_on		P1OUT |= BIT0
#define red_off		P1OUT &= ~BIT0
#define center_cutoff	484
#define left_cutoff		400
#define right_cutoff	375

//----------------------------------------------------------------------
//----------------------------------------------------------------------
int main(void) {

    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    initSensors();
    initMotors();
	req_func();

} // end main



void initSensors(void) {

    P1DIR &= ~BIT3;
    P1REN |= BIT3;

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

} // end initSensors



void initMotors(void) {

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

} // end initMotors



void req_func(void) {

	// Forward until first wall
	go_forward();
	//pause();
	turn_left();
	//pause();
	// Complete required functionality
	go_forward();
	pause();
	b_func();


} // end req_func



void b_func(void) {

	turn_right();

	// Complete B functionality
	go_forward();

	a_func();

} // end b_func



void a_func(void) {

	turn_right();

	go_forward();

	// Complete a functionality
	turn_left();
	go_forward();

} // end a_func



int check_sensors() {

	unsigned short left_readings[16], center_readings[16], right_readings[16];		// Just to analyze the values
	unsigned char i = 0;										// index into sample array

	IFG1=0; 													// clear interrupt flag1
	WDTCTL = WDTPW + WDTHOLD;									// disable WDT

	BCSCTL1 = CALBC1_8MHZ;										// 8MHz clock
	DCOCTL = CALDCO_8MHZ;

	// Right IR Sensor
	ADC10CTL0 = 0;											// Turn off ADC subsystem
	ADC10CTL1 = INCH_5 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
	ADC10AE0 = BIT5;		 								// Make P1.4 analog input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// Start a conversion
	while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
	right_readings[i] = ADC10MEM;									// collect that 10-bit value
	if (right_readings[i] > right_cutoff)	green_on;
	else					green_off;

	// Left IR Sensor
	ADC10CTL0 = 0;											// Turn off ADC subsystem
	ADC10CTL1 = INCH_3 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
	ADC10AE0 = BIT3;		 								// Make P1.4 analog input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// Start a conversion
	while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
	left_readings[i] = ADC10MEM;									// collect that 10-bit value
	if (left_readings[i] > left_cutoff)	red_on;
	else					red_off;

	// Center IR Sensor
	ADC10CTL0 = 0;											// Turn off ADC subsystem
	ADC10CTL1 = INCH_4 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
	ADC10AE0 = BIT4;		 								// Make P1.4 analog input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// Start a conversion
	while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
	center_readings[i] = ADC10MEM;									// collect that 10-bit value
	if (center_readings[i] > center_cutoff) {
		green_on;
		red_on;
		return 0;
	} else {
		green_off;
		red_off;
	}

	i = (i+1) & 0xF;							// This is a mod 16 increment

	return 1;

} // end check_sensors



void go_forward() {

	int wall = 0;

	//left motor
	P2OUT |= BIT0;						//enable left motor
	P2OUT |= BIT1;                     //move left wheel forward
	TA1CCTL1 = OUTMOD_7;

	//right motor
	P2OUT |= BIT5;						//enable right motor
	P2OUT &= ~BIT3;                     //move right wheel forward
	TA1CCTL2 = OUTMOD_3;

	while(wall == 0) {
		wall = check_sensors();
	}

} // end go_forward



void turn_left(void) {

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

} // end turn_left



void turn_right(void) {

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

} // end turn_right



void pause(void) {
	//disable both motors
	P2OUT &= ~BIT0;
	P2OUT &= ~BIT5;

	int i,k;
	for(i = 0; i < 9000; i++)
		{
			for(k = 0; k < 100; k++);
		}
} // end pause
