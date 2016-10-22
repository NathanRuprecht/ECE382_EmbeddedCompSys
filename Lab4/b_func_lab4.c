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

	unsigned char	x, y, y_vel, x_vel;

	// === Initialize system ================================================
	IFG1=0; /* clear interrupt flag1 */
	WDTCTL=WDTPW+WDTHOLD; /* stop WD */

	init();
	initNokia();
	clearDisplay();
	x=4;		y=4;	x_vel=1;	y_vel=1;
	drawBlock(y,x);

	while(1) {

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

		}
}
