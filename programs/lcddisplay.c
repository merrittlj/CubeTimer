/*  lcddisplay.c    */
/*  Author: Lucas Merritt/dynamic.void  */
/*  Date: 11/21/22  */
/*  Moving to a LCD now, tired of segmented displays    */


/*Hardware  Port    Pin*/
//Pin E     B       3
//Pin RS    B       4
//Pin D4    D       2
//Pin D5    D       3
//Pin D6    D       4
//Pin D7    D       5

#include <util/delay.h>

#define PORTB_DDR (*((volatile unsigned char *)0x24))
#define PORTD_DDR (*((volatile unsigned char *)0x2A))
#define PORTB_DATA (*((volatile unsigned char *)0x25))
#define PORTD_DATA (*((volatile unsigned char *)0x2B))

/*Initialization(maybe wait 1ms between instructions or something)*/
//Description, field, 7654 3210
//RS 0, configuration
//Set 4-bit mode, clear 4th bit, xxx0 xxxx
//Currently controlling x bits 7-4: xxxx yyyy(other bits for 8-bit)
//TODO: Add extra line to set to 2 line display and 5x10 dots - xxxx 11xx
//Clear display, set last bit, xxx1
//Return home, set first bit, xx1x
//Display on, no cursor, set the 2nd and 3rd bits, 11xx
//RS 1, data
//Write data, set specific bits, xxxx
//^ For HI, send 0100 && 1000 - 0100 && 1001
//NOTICE: Set E high for ~300us and then set it off after every step
//Steps:
//CMD1
//0000
//CMD2
//0000
//CMD3
//0000
//CMD4
//DATA1A
//DATA1B
//DATA2A
//DATA2B

//Enable to send data, disable
void sendPins() {
    //Set enable to high, 3rd PB pin
    PORTB_DATA |= (1<<3);
    //Wait at least 300us
    _delay_us(400);
    //Clear to low
    PORTB_DATA &= ~(1<<3);
}
//To make init faster
void sendCommand(unsigned char pin) {
    //Set supplied pin
    PORTD_DATA |= (1<<pin);
    //Send, clear, send
    sendPins();
    PORTD_DATA &= ~0x3C;
    sendPins();
}

int main() {
    /*Initialization(TODO: move to function)*/
    //Make all outputs(friendly)
    PORTB_DDR |= 0x18;
    PORTD_DDR |= 0x3C;
    //Clear RS, config
    PORTB_DATA &= ~(1<<4);
    //Send 1st command: 4 bit mode
    //Set hardware D5 pin 3
    sendCommand(3);
    //Send 2nd command: Clear display
    //Set hardware D4 pin 2
    sendCommand(2);
    //Send 3rd command: Return home
    //Hardware D5 pin 3
    sendCommand(3);
    //Don't need to clear after last command, easiest todo manually
    //Send 4th command: Display on, no cursor
    //Set hardware D6 && 7 pin 4 && 5
    PORTD_DATA |= (1<<4)+(1<<5);
    sendPins();
    
    //Set RS, data
    PORTB_DATA |= (1<<4);
    //TODO: make function that will display inputted character
    //Send 1st part of H, hardware D6 pin 4
    PORTD_DATA |= (1<<4);
    sendPins();
    //Send 2nd part of H, hardware D7 pin 5
    PORTD_DATA |= (1<<5);
    sendPins();
    //Send 1st part of I, hardware D6 pin 4
    PORTD_DATA |= (1<<4);
    sendPins();
    //Sned 2nd part of I, hardware D4 && D7 pin 2 && 5
    PORTD_DATA |= (1<<2)+(1<<5);
    sendPins();
}
