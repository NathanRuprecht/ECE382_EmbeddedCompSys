#include <msp430g2553.h>

extern void init();
extern void initNokia();
extern void clearDisplay();
extern void drawBlock(unsigned char row, unsigned char col);
extern void drawPaddle(unsigned char row, unsigned char col);

#define		TRUE			1
#define		FALSE			0
#define		UP_BUTTON		(P2IN & BIT5)
#define		DOWN_BUTTON		(P2IN & BIT4)
#define		AUX_BUTTON		(P2IN & BIT3)
#define		LEFT_BUTTON		(P2IN & BIT2)
#define		RIGHT_BUTTON	(P2IN & BIT1)


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
