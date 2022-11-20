/*	buttonled.c	*/
/*	Author: Lucas Merritt/dynamic.void	*/
/*	Date: 11/14/22	*/
/*	POC of knowledge + AU	*/


//Button: PC4
//LED: PD2

//IO 0x20-0x5F
//datad: offset 0xB
//ddrd: offset 0xA
//pind: offset 0x9

#define PORTD_DATA (*((volatile unsigned char *)0x2B))
#define PORTD_DDR (*((volatile unsigned char *)0x2A))
#define PORTD_PIN (*((volatile unsigned char *)0x29))

int main() {
	/*	Initialization	*/
	//Set direction of pins 1=out 0=in
	PORTD_DDR |= (1<<2);
	PORTD_DDR &= ~(1<<4);

	//Set LED to initially be off
	PORTD_DATA &= ~(1<<2);
	
	/*	Main Program Loop	*/
	while(1<2) {
		//Get button value and set LED to it
		PORTD_DATA |= (PORTD_PIN&(1<<4))>>2;
	}
}
