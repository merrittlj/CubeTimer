/*	sevenseg.c	*/
/*	Author: Lucas Merritt/dynamic.void	*/
/*	Date: 11/16/22	*/
/*	Turn on four digit seven segment display, POC	*/


/*Segment	Port	Pin*/
//A			D		2
//B			D		3
//C			D		4
//D			D		5
//E			D		6
//F			D		7
//G			B		0
//D1		B		1
//D2		B		2
//D3		B		3
//D4		B		4
//Decimal	A		5

#include <util/delay.h>

#define PORTD_DATA (*((volatile unsigned char *)0x2B))
#define PORTB_DATA (*((volatile unsigned char *)0x25))
#define PORTD_DDR (*((volatile unsigned char *)0x2A))
#define PORTB_DDR (*((volatile unsigned char *)0x24))

struct displayDigit {
	unsigned char displayedSegments;
	unsigned char gSegment;
};

char charD[] = "0123";

int main() {
	//Set directions(All output)
	PORTD_DDR |= 0xFC;
	PORTB_DDR |= 0x1F;
	//All displayable digits
	struct displayDigit displayed[10] = {
	{0xFC, 0},//0
	{0x18, 0},//1
	{0x6C, 1},//2
	{0x3C, 1},//3
	{0x96, 1},//4
	{0xB4, 1},//5
	{0xF4, 1},//6
	{0x1C, 0},//7
	{0xFC, 1},//8
	{0x9C, 1},//9
	};
	/*struct displayDigit nums[4];
	nums[0] = displayed[0];
	nums[1] = displayed[1];
	nums[2] = displayed[2];
	nums[3] = displayed[3];
	for(char i=0; i<4; i++) {
		nums[i] = displayed[(int)charD[i]];
	}*/
	//Create array(TODO: make nums based off of charD)
	struct displayDigit nums[4] = {
	{0xFC, 0},//0
	{0x18, 0},//1
	{0x6C, 1},//2
	{0x3C, 1},//3
	};
	//Display index
	char iL = 1;

	while(1<2) {
		//Clear display
		PORTD_DATA &= ~0xFC;
		PORTB_DATA &= ~0x1F;
		//Set display number
		PORTB_DATA |= ((1<<iL)^0x1E);
		//Set G segment
		//PORTB_DATA |= nums[iL-1].gSegment;
		//Set the rest of the segments
		//PORTD_DATA |= nums[iL-1].displayedSegments;
		PORTD_DATA |= 0xFC;
		//Check if index is maximum
		if(iL==4) {
			//Reset index
			iL = 1;
		}
		else {
			//Incrment index
			iL++;
		}
		//Wait(TODO: homemade delay function)
		//1s used for timing 1ms used for production
		_delay_ms(1000);
	}
}

