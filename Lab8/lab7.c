//-----------------------------------------------------------------
// Name:	C2C Nathan Ruprecht
// File:	lab7.c
// Date:	Fall 2014
// Purp:	Collect analog samples from P1.3, P1.4, and P1.5 and
//			turn the LEDs on and off depending on "wall" presence
//-----------------------------------------------------------------
#include "msp430g2553.h"

void initMSP430();

#define green_on	P1OUT |= BIT6
#define green_off	P1OUT &= ~BIT6
#define red_on		P1OUT |= BIT0
#define red_off		P1OUT &= ~BIT0

//----------------------------------------------------------------------
//----------------------------------------------------------------------
int main(void) {

	unsigned short sample3[16];									// Just to analyze the values
	unsigned short sample4[16];									// Just to analyze the values
	unsigned short sample5[16];									// Just to analyze the values
	unsigned char i = 0;										// index into sample array

	IFG1=0; 													// clear interrupt flag1
	WDTCTL = WDTPW + WDTHOLD;									// disable WDT

	BCSCTL1 = CALBC1_8MHZ;										// 8MHz clock
	DCOCTL = CALDCO_8MHZ;

	P1DIR = BIT0 | BIT6;												// Set the red LED as output

	int sensor = 0;
	while(1) {

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


	} // end infinite loop


} // end main
