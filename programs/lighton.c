/*	lighton.c	*/
/*	Author: Lucas Merritt/dynamic.void	*/
/*	Date: 11/14/22	*/
/*	Overview: Enable built-in LED on AU for POC	*/


//built-in LED PB5

#define PB_DATA (*((volatile unsigned long *)0x2B))
#define PB_DIR (*((volatile unsigned long *)0x2A))

int main() {
	//Set direction of built-in LED to output
	PB_DIR |= (1<<7);
	//Set output data of built-in LED to 1
	PB_DATA |= (1<<7);
}
