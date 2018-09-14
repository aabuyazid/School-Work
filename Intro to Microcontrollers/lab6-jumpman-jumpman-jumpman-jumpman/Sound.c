// Sound.c
// This module contains the SysTick ISR that plays sound
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
#include "dac.h"
#include "tm4c123gh6pm.h"
#include "Sound.h"				
const unsigned short SineWave[64] = {				
  32,35,38,41,44,47,49,52,54,56,58,				
  59,61,62,62,63,63,63,62,62,61,59,				
  58,56,54,52,49,47,44,41,38,35,				
  32,29,26,23,20,17,15,12,10,8,				
  6,5,3,2,2,1,1,1,2,2,3,				
  5,6,8,10,12,15,17,20,23,26,29};		
extern uint32_t Index;
// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(uint32_t period){
  DAC_Init (); 																										//DAC is PORTB
	Index =0; 																											// Not completely sure if I am supposed to be using NVIC or Systick Timers
	NVIC_ST_CTRL_R =0; 																							//Disable SysTick during setup 
	NVIC_ST_RELOAD_R= period -1; 																		//reload value
	NVIC_ST_CURRENT_R =0; 																					// clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;  //Priority 1
	NVIC_ST_CTRL_R = 0x0007; 																				// Enable Systick with core clock and interrupts. Not sure if 0x0007 is right or if it should be different for 6-bit DAC
}


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none
void Sound_Play(uint32_t period){
	NVIC_ST_RELOAD_R= period;
	// Value will change based on equation (80 MHz / (64 x the frequency of the sound wave being played))
// Does reload value depending on pitch?
}

void SysTick_Handler (void) {
	GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R ^ 0x02);      //ADDED CODE TO TOGGLE ANOTHER LED TO MAKE IT MINIMALLY INTRUSIVE; add a conditional as well to make it run so often
	DAC_Out (SineWave [Index]);    																// Define SineWave as an array
	Index = (Index +1) &0x3F;																			// Mask for 64
	
}

