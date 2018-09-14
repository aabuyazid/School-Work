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
#include "Interaction.h"
#include "tm4c123gh6pm.h"
#include  "Timer2.h"
#include "ST7735.h"
uint8_t ZombieTimer1Status;
void (*PeriodicTask2)(void);   // user function

extern Zombie Zombies[6];
extern Player Players[1];
extern Projectile Projectiles [10];
extern uint16_t *normalzombie;
// ***************** TIMER2_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2_Init( uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2
  //PeriodicTask2 = task;          // user function
  TIMER2_CTL_R = 0x00000000;    // 1) disable TIMER2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear TIMER2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x00000000; // 8) priority 4 THIS LINE
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

void Timer2A_Handler(void){
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;
/*	uint8_t i; 
	Player *ptplayer;
	Projectile *ptprojectile;
	Zombie *ptzombie;
	for(i=0;i<2;i++){
		ptplayer = &Players[i];
		if(Players[i].status==1){
			if((Players[i].prevx != Players[i].xcoor)||(Players[i].prevy != Players[i].ycoor)){
				if(Players[i].direction == up){
				
					ST7735_FillRect(Players[i].prevx,Players[i].prevy,  Players[i].width, Players[i].height,ST7735_BLACK);
					ST7735_DrawBitmap(Players[i].xcoor,Players[i].ycoor, Players[i].ptimageup, Players[i].width, Players[i].height);
				}
				else if(Players[i].direction == down){
					ST7735_FillRect(Players[i].prevx,Players[i].prevy-Players[i].height,  Players[i].width, Players[i].height,ST7735_BLACK);
					ST7735_DrawBitmap(Players[i].xcoor,Players[i].ycoor, Players[i].ptimagedown, Players[i].width, Players[i].height);
				}
				else if(Players[i].direction == left){
					ST7735_FillRect(Players[i].prevx,Players[i].prevy - Players[i].height,  Players[i].width, Players[i].height,ST7735_BLACK);
					ST7735_DrawBitmap(Players[i].xcoor,Players[i].ycoor, Players[i].ptimageleft, Players[i].width, Players[i].height);
				}
				else if(Players[i].direction == right){
					ST7735_FillRect(Players[i].prevx,Players[i].prevy - Players[i].height,  Players[i].width, Players[i].height,ST7735_BLACK);
					ST7735_DrawBitmap(Players[i].xcoor,Players[i].ycoor, Players[i].ptimageright, Players[i].width, Players[i].height);
				}
			}
		}
		if(Players[i].dead == 1){
			ST7735_FillRect(Players[i].xcoor,Players[i].ycoor, Players[i].width, Players[i].height, ST7735_BLACK);
			ptplayer->dead = 0;
		}
	}
	for(i=0;i<5;i++){
		ptzombie = &Zombies[i];
		if(Zombies[i].status==1){
			if((Zombies[i].prevx != Zombies[i].xcoor)||(Zombies[i].prevy != Zombies[i].ycoor)){
				if(Zombies[i].direction == up){
				
					ST7735_FillRect(Zombies[i].prevx,Zombies[i].prevy,  Zombies[i].width, Zombies[i].height,ST7735_BLACK);
					ST7735_DrawBitmap(Zombies[i].xcoor,Zombies[i].ycoor, normalzombie, Zombies[i].width, Zombies[i].height);
				}
				else if(Zombies[i].direction == down){
					ST7735_FillRect(Zombies[i].prevx,Zombies[i].prevy-Zombies[i].height,  Zombies[i].width, Zombies[i].height,ST7735_BLACK);
					ST7735_DrawBitmap(Zombies[i].xcoor,Zombies[i].ycoor, normalzombie, Zombies[i].width, Zombies[i].height);
				}
				else if(Zombies[i].direction == left){
					ST7735_FillRect(Zombies[i].prevx+ Zombies[i].width,Zombies[i].prevy - Zombies[i].height,  Zombies[i].width, Zombies[i].height,ST7735_BLACK);
					ST7735_DrawBitmap(Zombies[i].xcoor,Zombies[i].ycoor, normalzombie, Zombies[i].width, Zombies[i].height);
				}
				else if(Zombies[i].direction == right){
					ST7735_FillRect(Zombies[i].prevx- Zombies[i].width,Zombies[i].prevy - Zombies[i].height,  Zombies[i].width, Zombies[i].height,ST7735_BLACK);
					ST7735_DrawBitmap(Zombies[i].xcoor,Zombies[i].ycoor, normalzombie, Zombies[i].width, Zombies[i].height);
				}
				//I don't know how you passed the image in the array....
			//	ST7735_FillRect(Zombies[i].prevx,Zombies[i].prevy,  Zombies[i].width, Zombies[i].height,ST7735_BLACK);
				//ST7735_DrawBitmap(Zombies[i].xcoor,Zombies[i].ycoor, normalzombie, Zombies[i].width, Zombies[i].height);
			}
			else {
				ST7735_DrawBitmap(Zombies[i].xcoor,Zombies[i].ycoor, normalzombie, Zombies[i].width, Zombies[i].height);
			}
		}
		if(Zombies[i].dead == 1){
			ST7735_FillRect(Zombies[i].xcoor,Zombies[i].ycoor, Zombies[i].width, Zombies[i].height, ST7735_BLACK);
			ptzombie->dead = 0;
		}
	}
	for(i = 0; i < 10; i++){
		ptprojectile = &Projectiles[i];
		if(Projectiles[i].status == 1){
			if(Projectiles[i].direction == up){				
				ST7735_FillRect(Projectiles[i].prevx,Projectiles[i].prevy,  Projectiles[i].width, Projectiles[i].height,ST7735_BLACK);
			}
			else if(Projectiles[i].direction == down){
				ST7735_FillRect(Projectiles[i].prevx,Projectiles[i].prevy-Projectiles[i].height,  Projectiles[i].width, Projectiles[i].height,ST7735_BLACK);	
			}
			else if(Projectiles[i].direction == left){
				ST7735_FillRect(Projectiles[i].prevx+ Projectiles[i].width,Projectiles[i].prevy - Projectiles[i].height,  Projectiles[i].width, Projectiles[i].height,ST7735_BLACK);
			}
			else if(Projectiles[i].direction == right){
				ST7735_FillRect(Projectiles[i].prevx- Projectiles[i].width,Projectiles[i].prevy - Projectiles[i].height,  Projectiles[i].width, Projectiles[i].height,ST7735_BLACK);
			}
			ST7735_DrawBitmap(Projectiles[i].xcoor,Projectiles[i].ycoor, Projectiles[i].ptimagey, Projectiles[i].width, Projectiles[i].height);
		}
		if(Projectiles[i].dead == 1){
			ST7735_FillRect(Projectiles[i].xcoor,Projectiles[i].ycoor, Projectiles[i].width, Projectiles[i].height, ST7735_BLACK);
			ptprojectile->dead = 0;
		}
	}	
	Move_All_Objects();
	Player_Collision();
	Zombie_Collision();
	Projectile_Collision();*/
	// acknowledge TIMER2A timeout
  //(*PeriodicTask2)();                // execute user task
}
