/*	sevenseg.c
 *	Author: Lucas Merritt/dynamic.void
 *	Date: 11/16/22
 *	Spaghetti Code with mysterious bugs and if statements	*/


/* Hardware Implementation */
//Segment	Port	Pin
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

/* Memory Locations */
#define PORTB_DATA (*((volatile unsigned char *)0x25))
#define PORTC_DATA (*((volatile unsigned char *)0x28))
#define PORTD_DATA (*((volatile unsigned char *)0x2B))
#define PORTB_DDR (*((volatile unsigned char *)0x24))
#define PORTC_DDR (*((volatile unsigned char *)0x27))
#define PORTD_DDR (*((volatile unsigned char *)0x2A))
#define PORTC_PINS (*((volatile unsigned char *)0x26))
/* States in the FSM */
#define displayTime 0
#define startWait 1
#define startReady 2
#define timeRunning 3
#define debugging 4

char charD[] = "0123"; //Removing this breaks the code?

struct State {
    int displayEnabled; //Display on or off
    double waitMs; //Time to wait, ms
    int nextStates[2]; //The next states(sensors not activated, all sensors activated)
};
typedef const struct State sType;

int main() {
    /*      Initialization      */
	/* Set directions(output) */
	PORTB_DDR |= 0xF;
	PORTC_DDR |= 0x3;
	PORTD_DDR |= 0xFC;
    /* Clear directions(input) */
    PORTC_DDR &= ~(0xC);
    

    /*      Variables     */
    /* All of the displayable digits */
    unsigned char displayed[10][2] = {
    /* Segments to enable(PD), G segment enabled */
        {0xFC, 0}, //0
        {0x18, 0}, //1
        {0x6C, 1}, //2
        {0x3C, 1}, //3
        {0x98, 1}, //4
        {0xB4, 1}, //5
        {0xF4, 1}, //6
        {0x1C, 0}, //7
        {0xFC, 1}, //8
        {0x9C, 1}, //9
    };
	/* Numbers to display on the next iteration */
	unsigned char numsToDisplay[4][2] = {
	    {0xFC, 0},//0
	    {0xFC, 0},//0
	    {0xFC, 0},//0
	    {0xFC, 0},//0
	};
    /* Finite State Machine Declaration */
    sType FSM[5] = {
        {1, 0, {displayTime, startWait}}, //displayTime state
        {1, 550, {displayTime, startReady}}, //startWait state
        {0, 0, {timeRunning, startReady}}, //startReady state
        {1, 0, {timeRunning, displayTime}}, //timeRunning state
        {1, 0, {debugging, debugging}} //debugging state
    };

    /* Indexes */
	int displayIndex = 0; //Index of which display to show the current num on
    int displayStateIndex = timeRunning; //Finite state machine index && initial state
    
    /* Booleans */
	int precisionEnabled = 1; //Precision switch: 1 = two points of accuracy/precision 0 = 1 point of accuracy/extra time

    /* Counters */
	int msTimeCounter = 0000; //Somewhat primitive time counter, when time update index is done it increments
    int stateWaitCounter = 0; //How long to wait in the current state
	int timeUpdateCounter = precisionEnabled ? 10 : 100; //How often msTimeCounter is incremented, based on precision

    /* Time */
	int displaySwitchTime = 1; //Time before switching between displays

    while(1<2) {
        /* Clearing the display */
		PORTB_DATA &= ~0xF;
		PORTC_DATA &= ~0x3;
		PORTD_DATA &= ~0xFC;

        /*    Segment Configuration    */
		PORTB_DATA |= 0xF-(1<<displayIndex); //Set the current display based on the index
		
        /* Variables */
        int stateDisplayEnabled = FSM[displayStateIndex].displayEnabled; //Display status based on state
		unsigned char displayN = 0x0; //What number to display
        int setG = 0; //If the G segment is enabled or not

        /* Retrieve numbers from numsToDisplay
         * Weird method, unfixable bug besides this 
         * TODO: Fix */
		if(displayIndex==0){displayN=numsToDisplay[0][0];setG=numsToDisplay[0][1];};
		if(displayIndex==1){displayN=numsToDisplay[1][0];setG=numsToDisplay[1][1];};
		if(displayIndex==2){displayN=numsToDisplay[2][0];setG=numsToDisplay[2][1];};
		if(displayIndex==3){displayN=numsToDisplay[3][0];setG=numsToDisplay[3][1];};
        

        /*    Display Adjustment    */
        /* Set Segments */
		PORTD_DATA |= (displayN*stateDisplayEnabled); //Set all segments except G
		PORTC_DATA |= (setG*2*stateDisplayEnabled); //Set G segment
        
        /* Set Decimal Point */
		if(displayIndex==(2-precisionEnabled)){PORTC_DATA|=(1*stateDisplayEnabled);}; //Set decimal point dependent on precisionEnabled

        /* Clear display if necessary */
        PORTD_DATA &= (0xFC*stateDisplayEnabled); //Clear segments dependent on state
        PORTC_DATA &= (0xF*stateDisplayEnabled)+0xC; //Clear decimal point dependent on state


        /* Delay */
		_delay_ms(displaySwitchTime);

        /*    Update Variables    */
		displayIndex += (displayIndex==3 ? -3 : 1); //Set displayIndex appropriately

        if(displayStateIndex==timeRunning) { //Only update if state is timeRunning
            if(timeUpdateCounter!=0){timeUpdateCounter--;} //Decrement timeUpdateCounter if it's not finished
            else {
                /* Update Time Variables */
                timeUpdateCounter = precisionEnabled ? 10 : 100; //Reset the time update counter
                msTimeCounter++; //Update/increment the time counter
                
                /* Digits */
                int tempMsTimeCounter = msTimeCounter; //Create throwaray time variable
                for(int i=4; i!=0; i--) { //For each number in numsToDisplay
                    int digit = tempMsTimeCounter % 10; //Get last digit in time
                    tempMsTimeCounter /= 10; //Remove the last digit in time

                    /* Update numsToDisplay
                     * Strange way again due to mysterious bugs */
                    if(digit==0){numsToDisplay[i-1][0]=displayed[0][0];numsToDisplay[i-1][1]=displayed[0][1];};
                    if(digit==1){numsToDisplay[i-1][0]=displayed[1][0];numsToDisplay[i-1][1]=displayed[1][1];};
                    if(digit==2){numsToDisplay[i-1][0]=displayed[2][0];numsToDisplay[i-1][1]=displayed[2][1];};
                    if(digit==3){numsToDisplay[i-1][0]=displayed[3][0];numsToDisplay[i-1][1]=displayed[3][1];};
                    if(digit==4){numsToDisplay[i-1][0]=displayed[4][0];numsToDisplay[i-1][1]=displayed[4][1];};
                    if(digit==5){numsToDisplay[i-1][0]=displayed[5][0];numsToDisplay[i-1][1]=displayed[5][1];};
                    if(digit==6){numsToDisplay[i-1][0]=displayed[6][0];numsToDisplay[i-1][1]=displayed[6][1];};
                    if(digit==7){numsToDisplay[i-1][0]=displayed[7][0];numsToDisplay[i-1][1]=displayed[7][1];};
                    if(digit==8){numsToDisplay[i-1][0]=displayed[8][0];numsToDisplay[i-1][1]=displayed[8][1];};
                    if(digit==9){numsToDisplay[i-1][0]=displayed[9][0];numsToDisplay[i-1][1]=displayed[9][1];};
                }
            }
        }

        /* Update State */
        stateWaitCounter--; //Decrease wait(1 ms passed by default)

        if(stateWaitCounter<=0) { //If the state wait is up
            displayStateIndex = timeRunning; //Debugging purposes, TODO: Ultrasonic sensor code
            stateWaitCounter = FSM[displayStateIndex].waitMs; //Reset state wait counter dependent on state
        }
	}
}

