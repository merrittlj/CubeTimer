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

#define PORTB_DDR (*((volatile unsigned char *)0x24))
#define PORTD_DDR (*((volatile unsigned char *)0x2A))
#define PORTB_DATA (*((volatile unsigned char *)0x25))
#define PORTD_DATA (*((volatile unsigned char *)0x2B))

//Initialization(maybe wait 1ms between instructions or something)
//Set 4-bit mode, clear 4th bit, xxx0 xxxx
//Currently controlling x bits 7-4: xxxx yyyy(other bits for 8-bit)
//TODO: Add extra line to set to 2 line display and 5x10 dots - xxxx 11xx
//Clear display, set last bit, xxx1
//Return home, set first bit, xx1x
//Display on, no cursor, set the 2nd and 3rd bits, 11xx
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
