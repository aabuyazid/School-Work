// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: Arkan Abuyazid and Michael Hernandez
// Date Created: 
// Last Modified:  
// Lab number: 6


#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "IO.h"
//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {	
 SYSCTL_RCGCGPIO_R = 0x20;// --UUU-- Code to initialize PF4 and PF2
	uint8_t delay;
	delay |=10;
	GPIO_PORTF_DIR_R &= 0x00;
	GPIO_PORTF_DIR_R |= 0x04;
	GPIO_PORTF_PUR_R |= 0x10;
	GPIO_PORTF_DEN_R |= 0x14;
	
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
 GPIO_PORTF_DATA_R ^= 0x04; // --UUU-- PF2 is heartbeat
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
	uint16_t Switch; //Is this a Switch Debounce? Switch will have to be global to work.
	Switch = GPIO_PORTF_DATA_R;
	Switch &= 0x0010;
	while( Switch == 0){
		Switch = GPIO_PORTF_DATA_R;
		Switch &= 0x0010;
	}
 DelayWait10ms(2);// --UUU-- wait for release; delay for 20ms; and then wait for press
	while (Switch == 0x0010){
		Switch = GPIO_PORTF_DATA_R;
		Switch &= 0x0010;
	}
}  


void DelayWait10ms(uint32_t n){
	uint32_t volatile time;
  while(n){
    time = 727240*2/91;  // 10msec, not sure this is right for 80 MHZ clock
    while(time){
	  	time--;
    }
    n--;
  }
}
