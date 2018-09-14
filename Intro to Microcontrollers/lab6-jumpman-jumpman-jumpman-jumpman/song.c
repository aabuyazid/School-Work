// song.c
// This software configures the song
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "song.h"
uint16_t element;

struct Song{
	
	uint32_t note;
	uint32_t time;
	
};

struct Song irony[57] = {
	
	{B0, 0x011E8CCC}, {B,0x011E0000}, {0, 0x00008CCC}, {B, 0x023D1998}, {DF1, 0x011E8CCC}, {B, 0x011E8CCC}, 
	{0, 0x011E8CCC}, {A, 0x011E8CCC}, {0, 0x011E8CCC}, {AF,  0x023D1998}, {A, 0x011E8CCC}, {AF, 0x011E8CCC}, 
	{GF, 0x011E8CCC}, {E, 0x011E8CCC}, {0, 0x011E8CCC}, {E, 0x011E8CCC}, {GF, 0x011E8CCC}, {AF, 0x023D1998}, 
	{A, 0x011E8CCC}, {B, 0x011E8CCC}, {0, 0x011E8CCC}, {E, 0x011E8CCC}, {0, 0x011E8CCC}, {GF, 0x023D1998}, 
	{E, 0x011E8CCC}, {GF, 0x011E8CCC}, {A, 0x011E8CCC}, {AF, 0x011E8CCC}, {0, 0x011E8CCC},
	
	{B0, 0x011E8CCC}, {B,0x011E8000}, {0, 0x00000CCC}, {B, 0x023D1998}, {DF1, 0x011E8CCC}, {B, 0x011E8CCC}, 
	{0, 0x011E8CCC}, {A, 0x011E8CCC}, {0, 0x011E8CCC}, {AF, 0x023D1998}, {E1, 0x011E8CCC}, {EF1, 0x011E8CCC}, 
	{B, 0x23D0CCC}, {0, 0x00000CCC}, {B, 0x011E8CCC}, {AF, 0x011E8CCC}, {DF1, 0x047A3330}, {B, 0x011E8CCC}, {0, 0x011E8CCC}, 
	{GF, 0x011E8CCC}, {E, 0x011E8CCC}, {GF, 0x023D1998}, {AF, 0x023D1998}, {B, 0x023D1998}, {E, 0x023D0CCC}, {0, 0x00000CCC}, {E, 0x08F46660},
	{0, 0x08F46660}
		
};


void Song_Init(void){
	
	volatile uint8_t delay;
	SYSCTL_RCGC2_R |= 0x22;
	delay |= 10;
	SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF0
	GPIO_PORTF_DIR_R &= ~(0x11);
	GPIO_PORTF_DEN_R |= 0x11;
	GPIO_PORTF_PUR_R |= 0x11;
	
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = 0x00000000;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x20000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
	
	element = 0;
	return;
	
}

uint32_t Song_In(void){
	
	uint32_t data = GPIO_PORTF_DATA_R & 0x0011;
	return data;
	
}

void Song_Play(void){
	
	TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
	
}
	


void Song_Stop(void){
	
	TIMER0_CTL_R = 0x00000000;    // 10) disable	TIMER0A
	element = 0;
	
}
	


void Timer0A_Handler(void){
	
	TIMER0_ICR_R = 0x00000001;
	element++;
	
	if(element ==57){
		element =0;
	}
	
	TIMER0_TAILR_R = irony[element].time;
	Sound_Play(irony[element].note);
	
}
	


