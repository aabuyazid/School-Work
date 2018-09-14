// song.h
// This software configures song output
// Runs on LM4F120 or TM4C123
// Program written by: Michael Hernandez and Arkan Abuyazid
// Date Created: 3/6/17 
// Last Modified: 3/22/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********
#include <stdint.h>

// Header files contain the prototypes for public functions
// this file explains what the module does

// **************Song_Init*********************
// Initialize on-board switches 
// Input: none
// Output: none
// Initializes PORTF pins 0 and 4 for song play
void Song_Init(void);

// **************Song_In*********************
// input from on-board switches 
// Input: GPIO_PORTF_DATA_R bits 0 and 4
// Output: none
uint32_t Song_In(void);


void Song_Play(void);



void Song_Stop(void);
