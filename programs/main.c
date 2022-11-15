/*	buttonled.c	*/
/*	Author: Lucas Merritt/dynamic.void	*/
/*	Date: 11/14/22	*/
/*	POC of knowledge + AU	*/


//Button: PC4
//LED: PD2

//IO 0x20-0x5F
//datad: offset 0xB
//datac: offset 0x8
//ddrd: offset 0xA
//ddrc: offset 0x7

#define PORTD_DATA (*((volatile unsigned long *)0x2B))
#define PORTD_DDR (*((volatile unsigned long *)0x2A))
//#define PORTC_DATA (*((volatile unsigned long *)0x28))
//#define PORTC_DDR (*((volatile unsigned long *)0x27))

int main() {
	/*	Initialization	*/
	//Set direction of pins 1=out 0=in
	PORTD_DDR |= (1<<2);
	PORTD_DDR &= ~(1<<4);

	//Set LED to initially be off
	PORTD_DATA &= ~(1<<2);
	
	/*	Main Program Loop	*/
	while(1<2) {
		//Get button value
		unsigned long button = (PORTD_DATA&(1<<4))>>2;
		//Set LED to button value
		PORTD_DATA |= button;
		/*if (button>>2==0) {
			PORTD_DATA |= (1<<2);
		}
		else {
			PORTD_DATA &= ~(1<<2);
		}*/
	}
}
