// Timer2.c
// Runs on LM4F120/TM4C123
// Use TIMER2 in 32-bit periodic mode to request interrupts at a periodic rate
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
#include "Struct.h"
#include "tm4c123gh6pm.h"
extern Zombie Zombies[15];
extern Player Players[2];
void (*PeriodicTask4)(void);   // user function

// ***************** TIMER2_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer4_Init(void(*task)(void), uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x10;   // 0) activate TIMER4
  PeriodicTask4 = task;          
  TIMER4_CTL_R = 0x00000000;    
  TIMER4_CFG_R = 0x00000000;   
  TIMER4_TAMR_R = 0x00000002;   
  TIMER4_TAILR_R = period-1;    // 4) reload value
  TIMER4_TAPR_R = 0;            
  TIMER4_ICR_R = 0x00000001;    
  TIMER4_IMR_R = 0x00000001;  
  NVIC_PRI17_R = (NVIC_PRI17_R&0xFF00FFFF)|0x00800000; // 8) priority 4 THIS LINE
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN2_R = 0x00000040;           // 9) enable IRQ 23 in NVIC
 // TIMER4_CTL_R = 0x00000001;    // 10) enable TIMER4A
}

/*void Timer4A_Handler(void){
	Player *ptplayer = &Players[0];
  TIMER4_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER4A timeout
	ptplayer->reloading = 0;
  (*PeriodicTask4)();                // execute user task
  TIMER4_CTL_R = 0x00000000;    //disable timer
}*/
