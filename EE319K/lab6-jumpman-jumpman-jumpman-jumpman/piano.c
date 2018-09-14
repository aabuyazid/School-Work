// Piano.c
// This software configures the off-board piano keys
// Runs on LM4F120 or TM4C123
// Program written by: Arkan Abuyazid and Michael Hernandez
// Date Created: 3/6/17 
// Last Modified: 3/22/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "tm4c123gh6pm.h"

// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none

void Piano_Init(void){
	
	volatile uint8_t delay;
	SYSCTL_RCGC2_R |= 0x10;
	delay = 10;
	GPIO_PORTE_DIR_R &= 0xE0;
	GPIO_PORTE_DEN_R |= 0x1F;
	return;
	
}

// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2

uint32_t Piano_In(void){
	
	uint32_t data = GPIO_PORTE_DATA_R;
	uint32_t count = 1;
	if(data==0) {
		return 0;
		}
	while(data>1){
		data = data/2;
		count++;
	}
	if(count>5){
		return 0;
		}
	
  return count; 
}
