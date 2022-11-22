/*  lcddisplay.c    */
/*  Author: Lucas Merritt/dynamic.void  */
/*  Date: 11/21/22  */
/*  Moving to a LCD now, tired of segmented displays    */


/*Hardware  Port    Pin*/
//Pin RW    B       2
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
//Two lines, set D7, 1xxx
//Clear display, set last bit, xxx1
//Return home, set first bit, xx1x
//Display on, no cursor, set the 2nd and 3rd bits, 11xx
//RS 1, data
//Write data, set specific bits, xxxx
//^ For HI, send 0100 && 1000 - 0100 && 1001

//Enable to send data, disable
void writePin(int pin, int pin2) {
    //Write data
    PORTD_DATA |= (1<<pin);
    if(pin2 != 1337) {
        PORTD_DATA |= (1<<pin2);
    }
    //Set enable to high, 3rd PB pin
    PORTB_DATA |= (1<<3);
    _delay_us(1);
    //Clear to low
    PORTB_DATA &= ~(1<<3);
    _delay_us(1);
}
//Check if busy flag is ready
void checkBusy() {
    unsigned char busy_mirror;

    //Set RW mode to read(1)
    PORTB_DATA |= (1<<2);
    //Clear RS low
    PORTB_DATA &= ~(1<<4);
    //Clear data direction of D7(busy flag area) to input
    PORTD_DDR &= ~(1<<5);

    do {
        //Initialize busy flag mirror
        busy_mirror = 0;
        //Set E high
        PORTB_DATA |= (1<<3);
        
        _delay_us(1);
        //Set mirror to flag
        busy_mirror |= (PORTB_DATA&(1<<5));
        
        //Clear E low
        PORTB_DATA &= ~(1<<3);
        _delay_us(1);

        //??
        PORTB_DATA |= (1<<3);
        _delay_us(1);
        PORTB_DATA &= ~(1<<3);
        _delay_us(1);
    } while (busy_mirror);
    //Fix up
    //Clear RW to write(0)
    PORTB_DATA &= ~(1<<2);
    //Set D7 back to output
    PORTD_DDR |= (1<<5);
}
//Write a character to the screen
void writeCharacter(char input[]) {
    //Make sure everything is set correctly
    PORTB_DATA &= ~(1<<2);
    PORTB_DATA |= (1<<4);
    PORTB_DATA &= ~(1<<3);
    //Get first 4 numbers to send
    unsigned int send1 = input[0]+input[1]+input[2]+input[3];
    unsigned int send2 = input[4]+input[5]+input[6]+input[7];
    //Write first batch of data
    writePin(send1,1337);
    writePin(send2,1337);

}

int main() {
    /*Initialization(TODO: move to function)*/
    //Make all outputs(friendly)
    PORTB_DDR |= 0x28;
    PORTD_DDR |= 0x3C;
    //Powerup delay
    _delay_ms(100);
    //Clear RS, config
    PORTB_DATA &= ~(1<<4);

    //4 bit mode, set pin 3
    writePin(3,1337);
    checkBusy();
    //Two lines, set pin 5
    writePin(5,1337);
    checkBusy();
    //Clear display, set pin 2
    writePin(2,1337);
    checkBusy();
    //Return home, set pin 3
    writePin(3,1337);
    checkBusy();
    //Display on, no cursor, set pin 4 && 5
    writePin(4,5);
    checkBusy();

    _delay_ms(100);
    
    //Set RS, data
    PORTB_DATA |= (1<<4);
    //TODO: make function that will display inputted character
    //Send 1st part of H, hardware D6 pin 4
    writePin((1<<2),1337);
    //Send 2nd part of H, hardware D7 pin 5
    writePin((1<<3),1337);
    //Send 1st part of I, hardware D6 pin 4
    writePin((1<<2),1337);
    //Sned 2nd part of I, hardware D4 && D7 pin 2 && 5
    writePin((1<<3),1);
}
