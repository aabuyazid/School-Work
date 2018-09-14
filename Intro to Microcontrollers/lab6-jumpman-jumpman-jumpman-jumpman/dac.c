// dac.c
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: Arkan Abuyazid and Michael Hernandez
// Date Created: 3/6/17 
// Last Modified: 3/22/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "tm4c123gh6pm.h"
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x02;
	uint8_t delay;
	delay |= 10;
	delay |=10;
	GPIO_PORTB_DIR_R &= 0x00;
  GPIO_PORTB_DIR_R |= 0x3F;
	GPIO_PORTB_DEN_R |= 0x3F;
	GPIO_PORTB_DATA_R = 0;
}

// **************DAC_Out*********************
// output to DAC
// Input: 6-bit data, 0 to 63 
// Output: none
void DAC_Out(uint32_t data){
	GPIO_PORTB_DATA_R = data;
}
