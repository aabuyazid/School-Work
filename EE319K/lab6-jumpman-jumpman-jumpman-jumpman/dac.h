// dac.h
// This software configures DAC output
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********
#include <stdint.h>
// Header files contain the prototypes for public functions
// this file explains what the module does

// **************DAC_Init*********************
// Initialize 6-bit DAC, called once 
// Input: none
// Output: none
// Initializes PORTB pins 0-6 for DAC
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC; only used when interrupted, never called in main
// Input: 6-bit data, 0 to 63 
// Output: none
void DAC_Out(uint8_t  data);
