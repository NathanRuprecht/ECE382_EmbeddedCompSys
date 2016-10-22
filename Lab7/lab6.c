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
