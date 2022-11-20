/*	sevenseg.c	*/
/*	Author: Lucas Merritt/dynamic.void	*/
/*	Date: 11/16/22	*/
/*	POC turned main project file	*/


/*Segment	Port	Pin*/
//D1		B		0
//D2		B		1
//D3		B		2
//D4		B		3
//Decimal	C		0
//G			C		1
//A			D		2
//B			D		3
//C			D		4
//D			D		5
//E			D		6
//F			D		7

#include <util/delay.h>

#define PORTB_DATA (*((volatile unsigned char *)0x25))
#define PORTC_DATA (*((volatile unsigned char *)0x28))
#define PORTD_DATA (*((volatile unsigned char *)0x2B))
#define PORTB_DDR (*((volatile unsigned char *)0x24))
#define PORTC_DDR (*((volatile unsigned char *)0x27))
#define PORTD_DDR (*((volatile unsigned char *)0x2A))

//Struct for displayable characters
struct displayChar {
	unsigned char displayedSegments;
	unsigned char gSegment;
};

//All displayable digits
struct displayChar displayed[10] = {
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

char charD[] = "0123";

int main() {
	//Set directions(All output)
	PORTB_DDR |= 0xF;
	PORTC_DDR |= 0x3;
	PORTD_DDR |= 0xFC;
	//Create array(TODO: make nums based off of charD)
	struct displayChar nums[4] = {
	{0xFC, 0},//0
	{0x18, 0},//1
	{0x6C, 1},//2
	{0x3C, 1},//3
	};
	//Display index
	char iL = 0;

	while(1<2) {
		//Clear display
		PORTB_DATA &= ~0xF;
		PORTC_DATA &= ~0x3;
		PORTD_DATA &= ~0xFC;
		//Set display number
		PORTB_DATA |= 0xF-(1<<iL);
		//Set the rest of the segments
		//PORTD_DATA |= nums[iL].displayedSegments;
		PORTD_DATA |= 0xFC;
		//Set index appropiately
		iL += (iL==3 ? -3 : 1);
		//Wait(TODO: homemade delay function)
		//1s used for debugging 1ms used for production
		_delay_ms(1000);
	}
}

