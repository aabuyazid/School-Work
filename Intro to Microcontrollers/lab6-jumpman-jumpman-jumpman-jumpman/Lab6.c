// Lab6.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: Arkan Abuyazid and Michael Hernandez
// Date Created: 3/6/17 
// Last Modified: 3/22/17 
// Lab number: 6
// Hardware connections
//  breadboard


#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "song.h"

uint32_t input;
uint32_t input1;
uint32_t previous;
uint32_t Index; 
uint16_t check=0;
uint8_t playing;


															// basic functions defined at end of startup.s
void DisableInterrupts(void); 													// Disable interrupts
void EnableInterrupts (void);  												  // Enable interrupts
void Heart_Beat_Init  (void);
void Heart_Beat 			(void);

int main(void){      
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);     // bus clock at 80 MHz
  Piano_Init();
  Sound_Init(0);
	Song_Init();
	Heart_Beat_Init();
	
  // other initialization
  EnableInterrupts();																	  // Do I move this line to within the while loop?
	previous = 0;
	playing = 0;
	
  while(1){ 
		Heart_Beat(); 
		input = Piano_In () & 0x1F;  												// Bit 0-6 for key hits
		input1 = Song_In();
		
		if(playing == 0){
		
			if(input&& (previous == 0)){
				EnableInterrupts();
				if(input ==1){
				Sound_Play(E);
				}
				if(input ==2){
				Sound_Play(GF);
				}
				if(input ==3){
				Sound_Play(AF);
				}
				if(input ==4){
				Sound_Play(A);
				}
				if(input ==5){
				Sound_Play(B);
				}
		   																	// Value will change based on equation (80 MHz / (64 x the frequency of the sound wave being played))
			}
				if (previous&&(input==0)){
				Sound_Init (0); 
				DisableInterrupts ();       												//Stop Sound
			}
			previous = input;
			
			if(input1 == 0x01){
				playing = 1;
				EnableInterrupts();
				Song_Play();
			}
			
		}
		else{
			
			if((input1 == 0x10)||(input1 == 0x00)){
				Song_Stop();
				playing = 0;
				DisableInterrupts();
				
			}
		}
  }    
}

void Heart_Beat_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	uint8_t delay;
	delay |= 10;
	//GPIO_PORTF_DIR_R &= 0x00;
  GPIO_PORTF_DIR_R |= 0x06;
	GPIO_PORTF_AFSEL_R &= 0x00;
	GPIO_PORTF_DEN_R |= 0x06;
}

void Heart_Beat (){
	if( check==10000){																				// Changes LED every 10000 times
		check= 0;
	  GPIO_PORTF_DATA_R = (GPIO_PORTF_DATA_R ^ 0x04);
		}
	check++;
}
