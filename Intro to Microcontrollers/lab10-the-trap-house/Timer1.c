// Timer1.c
// Runs on LM4F120/TM4C123
// Use TIMER1 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// Last Modified: 3/6/2015 
// You can use this timer only if you learn how it works

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
  Program 7.5, example 7.6

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>
#include "ADC.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "Interaction.h"
uint32_t results[2];
extern Player Players[2];
void (*PeriodicTask1)(void);   // user function

// ***************** TIMER1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1_Init( uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1 CHANGES
  //PeriodicTask1 = task;          // user function
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0;            // 5) bus clock resolution
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF00FF)|0x00008000; // 8) priority 4 CHANGES
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC  CHANGES
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

//MC2
/*
void Timer1A_Handler(void){
	uint32_t k=0, l=0;
	char coordinatesJS=0x02;
	UART1_OutChar(coordinatesJS);
	CoordPot temp =	ADC_In();
	k= temp.x;
	l= temp.y;
	coordinatesJS=0x41;
	UART1_OutChar( coordinatesJS);
	coordinatesJS=0x30;
	if(((k>1500)&&(k<2800))&&((l>1500)&&(l<2800))){
		coordinatesJS=0x30;
	}
	if((k>1500)&&(k<2800)){
		if(l<1500){
	  	coordinatesJS=0x32;
		}
		if(l>2800){
	  	coordinatesJS=0x34;
		}
	}
	if((l>1500)&&(l<2800)){
		if(k<1500){
	 		coordinatesJS=0x33;
		}
		if(k>2800){
	 		coordinatesJS=0x31;
		}
	}
	if(((k<500)&&(k>2800))&&((l<1500)&&(l>2800))){
		coordinatesJS=0x30;	
	}
	UART1_OutChar( coordinatesJS);
	coordinatesJS=0x03;
	UART1_OutChar( coordinatesJS);
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
  //(*PeriodicTask1)();                // execute user task
}*/

//MC1
void Timer1A_Handler(void){
	Player *ptplayer = &Players[1];
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;
	ptplayer->reloading = 0;

	// (*PeriodicTask1)();                // execute user task
	TIMER1_CTL_R = 0x00000000;// acknowledge TIMER1A timeout
}
