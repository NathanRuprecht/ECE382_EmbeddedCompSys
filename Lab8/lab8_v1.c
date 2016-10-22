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
void check_sensors();

//----------------------------------------------------------------------
//----------------------------------------------------------------------
int main(void) {

    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P1DIR &= ~BIT3;
    P1REN |= BIT3;

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

	req_func();
	b_func();
	a_func();

} // end main



void req_func(void) {

	int sample4 = 0x0

	while (sample4 < 0x100) {
		go_forward();
		sample4 = check_sensors();
	}

	turn_left();


} // end req_func



void b_func(void) {

	int sample4 = 0x0

	while (sample4 < 0x100) {
		go_forward();
		sample4 = check_sensors();
	}

	turn_right();
	go_forward();

} // end b_func



void a_func(void) {

	int sample4 = 0x0

	while (sample4 < 0x100) {
		go_forward();
		sample4 = check_sensors();
	}

	turn_right();

	while (sample4 < 0x100) {
		go_forward();
		sample4 = check_sensors();
	}

	turn_left();
	go_forward();

} // end a_func



void check_sensors(void) {

	unsigned short sample3[16];									// Just to analyze the values
	unsigned short sample4[16];									// Just to analyze the values
	unsigned short sample5[16];									// Just to analyze the values
	unsigned char i = 0;										// index into sample array

	IFG1=0; 													// clear interrupt flag1
	WDTCTL = WDTPW + WDTHOLD;									// disable WDT

	BCSCTL1 = CALBC1_8MHZ;										// 8MHz clock
	DCOCTL = CALDCO_8MHZ;

	int sensor = 0;

	switch(sensor) {
				case 0: // Configure P1.3 to be the ADC input for LEFT sensor

					ADC10CTL0 = 0;											// Turn off ADC subsystem
					ADC10CTL1 = INCH_3 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
					ADC10AE0 = BIT3;		 								// Make P1.4 analog input
					ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

					ADC10CTL0 |= ADC10SC;									// Start a conversion
					while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete

					sample3[i] = ADC10MEM;									// collect that 10-bit value

					if (ADC10MEM > 0x0200)
						green_on;
					else
						green_off;

					i = (i+1) & 0xF;							// This is a mod 16 increment

					sensor = 1;
					break;

				case 1: // Configure P1.4 to be the ADC input for CENTER sensor

					ADC10CTL0 = 0;											// Turn off ADC subsystem
					ADC10CTL1 = INCH_4 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
					ADC10AE0 = BIT4;		 								// Make P1.4 analog input
					ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

					ADC10CTL0 |= ADC10SC;									// Start a conversion
					while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete

					sample4[i] = ADC10MEM;									// collect that 10-bit value

					if (ADC10MEM > 0x0200) {
						red_on;
						green_on;
					} else {
						red_off;
						green_off;
					}

					i = (i+1) & 0xF;							// This is a mod 16 increment

					sensor = 2;
					break;

				case 2: // Configure P1.5 to be the ADC input for RIGHT sensor

					ADC10CTL0 = 0;											// Turn off ADC subsystem
					ADC10CTL1 = INCH_5 | ADC10DIV_3 ;						// Channel 4, ADC10CLK/4
					ADC10AE0 = BIT5;		 								// Make P1.4 analog input
					ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

					ADC10CTL0 |= ADC10SC;									// Start a conversion
					while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete

					sample5[i] = ADC10MEM;									// collect that 10-bit value

					if (ADC10MEM > 0x0200)
						red_on;
					else
						red_off;

					i = (i+1) & 0xF;							// This is a mod 16 increment

					sensor = 0;
					break;

			} //end switch

} // end check_sensors



void go_forward(void) {

	// Go Forward
		TA1CCR1 = 0x00C0;
		TA1CCTL1 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
		P2OUT |= BIT1;

		TA1CCR2 = 0x00C0;
		TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
		P2OUT |= BIT3;

		_delay_cycles(500000);

} // end go_forward



void turn_left(void) {

    // Big left
    TA1CCR1 = 0x00C0;
    TA1CCTL1 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
    P2OUT |= BIT1;

    TA1CCR2 = 0x00C0;
    TA1CCTL2 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode
    P2OUT &= ~BIT3;

    _delay_cycles(500000);

} // end turn_left



void turn_right(void) {

    TA1CCR1 = 0x00C0;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode
    P2OUT &= ~BIT1;

    TA1CCR2 = 0x00C0;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode
    P2OUT |= BIT3;

    _delay_cycles(500000);

} // end turn_right
