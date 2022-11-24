/*	sevenseg.c	*/
/*	Author: Lucas Merritt/dynamic.void	*/
/*	Date: 11/16/22	*/
/*	Spaghetti Code with mysterious bugs and if statements	*/


/*Segment	Port	Pin*/
//D1		B		0
//D2		B		1
//D3		B		2
//D4		B		3
//Decimal	C		0
//G			C		1
//Sensor1   C       2
//Sensor2   C       3
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
#define PORTC_PINS (*((volatile unsigned char *)0x26))
#define displayTime 0
#define startWait 1
#define startReady 2
#define timeRunning 3

//...
//if I even change the name of this it screws everything up
//when I tried to change it to "ihatemylife" it started messing up my 3rd digit
//what??
char charD[] = "0123";

struct State {
    //Display on or off
    int displayEnabled;
    //Time to wait, ms
    double waitMs;
    //The next states(sensors not activated, all sensors activated)
    int nextStates[2];
};
typedef const struct State sType;

int main() {
	//Set directions(output)
	PORTB_DDR |= 0xF;
	PORTC_DDR |= 0x3;
	PORTD_DDR |= 0xFC;
    //Clear directions(input)
    PORTC_DDR &= ~(0xC);
    //All displayable digits
    unsigned char displayed[10][2] = {
        {0xFC, 0},//0
        {0x18, 0},//1
        {0x6C, 1},//2
        {0x3C, 1},//3
        {0x98, 1},//4
        {0xB4, 1},//5
        {0xF4, 1},//6
        {0x1C, 0},//7
        {0xFC, 1},//8
        {0x9C, 1},//9
    };
	//Create array
	unsigned char nums[4][2] = {
	    {0xFC, 0},//0
	    {0xFC, 0},//0
	    {0xFC, 0},//0
	    {0xFC, 0},//0
	};
	//Display index
	int iL = 0;
	//Time
	int timeMs = 0000;
    //Finite state machine
    sType FSM[4] = {
        //displayTime state
        {1, 0, {displayTime, startWait}},
        //startWait state
        {1, 550, {displayTime, startReady}},
        //startReady state
        {0, 0, {timeRunning, startReady}},
        //timeRunning state
        {1, 0, {timeRunning, displayTime}}
    };
    //State machine index
    int stateIndex = displayTime;
	//Precision/extra time switch: TODO: implement on hardware
	int precision = 1;
	//Digit update rate(time before switching to different display), ms
	int delayT = 1;
	//Time update index
	int timeUpdate = precision ? 10 : 100;
    //State wait index
    int stateWait = 0;
    while(1<2) {
		//Clear display
		PORTB_DATA &= ~0xF;
		PORTC_DATA &= ~0x3;
		PORTD_DATA &= ~0xFC;
		//Set display number
		PORTB_DATA |= 0xF-(1<<iL);
		//Set all of the segments(really crappy manner)
		unsigned char displayN = 0x0;
        unsigned char setG = 0;
		if(iL==0){displayN=nums[0][0];setG=nums[0][1];};
		if(iL==1){displayN=nums[1][0];setG=nums[1][1];};
		if(iL==2){displayN=nums[2][0];setG=nums[2][1];};
		if(iL==3){displayN=nums[3][0];setG=nums[3][1];};
        //Display status
        int displayOn = FSM[stateIndex].displayEnabled;
        //Set all segments except G
		PORTD_DATA |= (displayN*displayOn);
        //Set G segment
		PORTC_DATA |= (setG*2*displayOn);
		//Set decimal point(depends on precision switch)
		if(iL==(2-precision)){PORTC_DATA|=(1*displayOn);};
		//Set index appropriately
		iL += (iL==3 ? -3 : 1);
        //Set display status depending on state
        PORTD_DATA &= (0xFC*displayOn);
        //Clear decimal point depending on state
        PORTC_DATA &= (0xF*displayOn)+0xC;
        //Wait(TODO: homemade delay function, maybe?)
		_delay_ms(delayT);
        //Decrement time update index and check if 0
        if(stateIndex==timeRunning) {
            if(timeUpdate!=0){timeUpdate--;}
            else {
                //Reset index
                timeUpdate = precision ? 10 : 100;
                //Update time
                timeMs++;
                //Create temp version of time to get individual digits
                int tempTimeMs = timeMs;
                for(int i=4; i!=0; i--) {
                    //Get last digit in number
                    int digit = tempTimeMs % 10;
                    //I really hope this doesn't work and isn't the way to fix this mysterious bug that I've spent way too long trying to fix
                    if(digit==0){nums[i-1][0]=displayed[0][0];nums[i-1][1]=displayed[0][1];};
                    if(digit==1){nums[i-1][0]=displayed[1][0];nums[i-1][1]=displayed[1][1];};
                    if(digit==2){nums[i-1][0]=displayed[2][0];nums[i-1][1]=displayed[2][1];};
                    if(digit==3){nums[i-1][0]=displayed[3][0];nums[i-1][1]=displayed[3][1];};
                    if(digit==4){nums[i-1][0]=displayed[4][0];nums[i-1][1]=displayed[4][1];};
                    if(digit==5){nums[i-1][0]=displayed[5][0];nums[i-1][1]=displayed[5][1];};
                    if(digit==6){nums[i-1][0]=displayed[6][0];nums[i-1][1]=displayed[6][1];};
                    if(digit==7){nums[i-1][0]=displayed[7][0];nums[i-1][1]=displayed[7][1];};
                    if(digit==8){nums[i-1][0]=displayed[8][0];nums[i-1][1]=displayed[8][1];};
                    if(digit==9){nums[i-1][0]=displayed[9][0];nums[i-1][1]=displayed[9][1];};
                    //...
                    //Remove the last digit for next iteration
                    tempTimeMs /= 10;
                }
            }
        }
        stateWait--;
        //Check if wait is up
        if(stateWait<=0) {
            //Reset wait
            stateWait = FSM[stateIndex].waitMs;
            //Check sensors
            //Set sensor1, pin 2
            unsigned char sensor1 = (PORTC_DATA&0x4);
            //Set sensor2, pin 3
            unsigned char sensor2 = (PORTC_DATA&0x8);
            //If both sensors activated
            if((sensor1+sensor2)==0xC) {
                //Change state
                stateIndex = FSM[stateIndex].nextStates[1];
            }
            //If not all of the sensors activated
            else {
                //Change state
                stateIndex = FSM[stateIndex].nextStates[0];
            }
            //Debugging states
            //stateIndex = displayTime;
        }
	}
}

