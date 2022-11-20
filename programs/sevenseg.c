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

//All displayable digits
unsigned char displayed[10][2] = {
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
	unsigned char nums[4][2] = {
	{0xFC, 0},//0
	{0x18, 0},//1
	{0x6C, 1},//2
	{0x3C, 1},//3
	};
	//Display index
	unsigned char iL = 0;

	while(1<2) {
		//Clear display
		PORTB_DATA &= ~0xF;
		PORTC_DATA &= ~0x3;
		PORTD_DATA &= ~0xFC;
		//Set display number
		PORTB_DATA |= 0xF-(1<<iL);
		//Set the rest of the segments(really crappy manner)
		unsigned char displayN = 0x0;
		if(iL==0){displayN=nums[0][0];};
		if(iL==1){displayN=nums[1][0];};
		if(iL==2){displayN=nums[2][0];};
		if(iL==3){displayN=nums[3][0];};
		PORTD_DATA |= displayN;
		//Set G segment
        unsigned char setG = 0;
		if(iL==0){setG=nums[0][1];};//WHY DOESN'T NUMS[iL][1] WORK
		if(iL==1){setG=nums[1][1];};//THERE IS NO REASON WHY IT SHOULDN'T
		if(iL==2){setG=nums[2][1];};//I HATE ARDUINO
		if(iL==3){setG=nums[3][1];};//I hope my future job employer doesn't see this
		PORTC_DATA |= (setG*2);
		//Set index appropriately
		iL += (iL==3 ? -3 : 1);
		//Wait(TODO: homemade delay function)
		//1s used for debugging 1ms used for production
		_delay_ms(1);
	}
}

