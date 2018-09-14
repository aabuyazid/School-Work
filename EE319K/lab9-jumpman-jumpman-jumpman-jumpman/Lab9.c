// Lab9.c
// Runs on LM4F120 or TM4C123
// Student names: Michael Hernandez and Arkan Abuyazid
// Last modification date: 
// Last Modified: 4/10/2017 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats
// UART1 on PC4-5
// * Start with where you left off in Lab8. 
// * Get Lab8 code working in this project.
// * Understand what parts of your main have to move into the UART1_Handler ISR
// * Rewrite the SysTickHandler
// * Implement the s/w FiFo on the receiver end 
//    (we suggest implementing and testing this first)

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "UART.h"
#include "FiFo.h"
uint8_t Min[10];
uint32_t TxCounter=0;
//*****the first three main programs are for debugging *****
// main1 tests just the ADC and slide pot, use debugger to see data
// main2 adds the LCD to the ADC and slide pot, ADC data is on Nokia
// main3 adds your convert function, position data is no Nokia

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PLL_Init(void);
void LCD_Init (void);
void SysTick_Init (void);
void Decimal_Conversion (uint32_t x);

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
uint32_t Data;      // 12-bit ADC
uint32_t Position;  // 32-bit fixed-point 0.001 cm


// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
	uint32_t  delayd;
	
  SYSCTL_RCGCGPIO_R|= SYSCTL_RCGC2_GPIOF;  //NEED TO CHANGE THIS
	for (delayd =0; delayd <100; delayd++){}
	GPIO_PORTF_DIR_R |= 0x0E;             
  GPIO_PORTF_AFSEL_R &= ~0x0E;          
  GPIO_PORTF_DEN_R |= 0x0E;             
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;
	PF1 = 0;
  PF2 = 0;       
  PF3 = 0;	
	// Intialize PortF for hearbeat
}

// Get fit from excel and code the convert routine with the constants
// from the curve-fit
uint32_t Convert(uint32_t input){
	uint32_t Pos;
	Pos = (((5 * input) + 502) / 10); // THIS IS NOT RIGHT FORMULA
  return Pos; //replace with your calibration code
}


// final main program for bidirectional communication
// Sender sends using SysTick Interrupt
// Receiver receives using RX
int main1(void){
	PLL_Init ();
	DisableInterrupts();
	SysTick_Init();
	DisableInterrupts();
	ADC_Init();
	UART_Init ();
	DisableInterrupts();
	PortF_Init();
	char *datapt; 
	EnableInterrupts();
	while (1){
		while(*datapt != 0x02){
			FiFo_Get(datapt);
		}
		while(*datapt != 0x03){
			if(*datapt == 0x0D){									//"/r"
				ST7735_OutChar(0x20);					//Space
				ST7735_OutChar(0x63);					//c
				ST7735_OutChar(0x6D);					//m
			}
			else{
				ST7735_OutChar(*datapt);
			}
		}
	}
}
int main(void){ 
	uint32_t n;
	uint32_t z;
  DisableInterrupts();
  PLL_Init ();
  TExaS_Init();       // Bus clock is 80 MHz 
  ST7735_InitR(INITR_REDTAB);
  ADC_Init();    // initialize to sample ADC
  PortF_Init();
  UART_Init();       // initialize UART
	ST7735_SetCursor(0,0);
  LCD_OutFix(0);
  ST7735_OutString(" cm");
	SysTick_Init();
//Enable SysTick Interrupt by calling SysTick_Init()
  char ch;
		char *datapt = &ch;
  EnableInterrupts();
  while(1){
		FiFo_Get(datapt);
		while(*datapt != 0x02){
			FiFo_Get(datapt);
			//ST7735_OutChar(*datapt);
		}
		ST7735_SetCursor(0,0);	
			//for( n=1; n<6; n++){
			//datapt+=n;
		while(*datapt != 0x03){
			
			if(*datapt == 0x0D){									//"/r"
				ST7735_OutChar(0x20);					//Space
				ST7735_OutChar(0x63);					//c
				ST7735_OutChar(0x6D);					//m
			}
			else{
				ST7735_OutChar(*datapt);
			}
			FiFo_Get(datapt);
		}
		//	FiFo_Get(datapt);
		//	FiFo_Get (datapt);
		//	ST7735_OutChar(*datapt);
		//}
	}
		
		//--UUU--Complete this  - see lab manual
	}
//}


void Decimal_Conversion (uint32_t x){
	uint32_t n=0;
	uint32_t divider= 1000;
	uint32_t i=0;
  Min[i]= 0x02 ;
	n = x;
	for(i=1; i<6; i++){
		if( i !=2){
			Min[i] = n/divider + 0x30;
			n = n%divider;
			divider = divider/10;
		}
		if (i ==2){
			Min[i]= 0x2E;
		}	
	}
	Min [i] =  0x0D;
	i++;
	Min [i] =  0x03;
	return ;
}
/* SysTick ISR
*/
void SysTick_Handler(void){ // every 25 ms
	uint32_t i=0;
	uint32_t sample=0;
	uint32_t distance =0;
	char MSG;
 	PF1 ^= 0x02;
	sample =ADC_In();
	PF1 ^= 0x02;
	distance= Convert(sample);
	Decimal_Conversion (distance);
	//MSG = Min[i];
	for (i=0;i<8; i++){
		MSG = Min[i];
		UART_OutChar( MSG);
	}
	TxCounter++;
  PF1 ^= 0x02;
	
	//Similar to Lab9 except rather than grab sample and put in mailbox
 //        format message and transmit 
}


uint32_t Status[20];             // entries 0,7,12,19 should be false, others true
char GetData[10];  // entries 1 2 3 4 5 6 7 8 should be 1 2 3 4 5 6 7 8
int mainfifo(void){ // Make this main to test FiFo
  FiFo_Init(); // Assuming a buffer of size 6
  for(;;){
    Status[0]  = FiFo_Get(&GetData[0]);  // should fail,    empty
    Status[1]  = FiFo_Put(1);            // should succeed, 1 
    Status[2]  = FiFo_Put(2);            // should succeed, 1 2
    Status[3]  = FiFo_Put(3);            // should succeed, 1 2 3
    Status[4]  = FiFo_Put(4);            // should succeed, 1 2 3 4
    Status[5]  = FiFo_Put(5);            // should succeed, 1 2 3 4 5
    Status[6]  = FiFo_Put(6);            // should succeed, 1 2 3 4 5 6
    Status[7]  = FiFo_Put(7);            // should fail,    1 2 3 4 5 6 
    Status[8]  = FiFo_Get(&GetData[1]);  // should succeed, 2 3 4 5 6
    Status[9]  = FiFo_Get(&GetData[2]);  // should succeed, 3 4 5 6
    Status[10] = FiFo_Put(7);            // should succeed, 3 4 5 6 7
    Status[11] = FiFo_Put(8);            // should succeed, 3 4 5 6 7 8
    Status[12] = FiFo_Put(9);            // should fail,    3 4 5 6 7 8 
    Status[13] = FiFo_Get(&GetData[3]);  // should succeed, 4 5 6 7 8
    Status[14] = FiFo_Get(&GetData[4]);  // should succeed, 5 6 7 8
    Status[15] = FiFo_Get(&GetData[5]);  // should succeed, 6 7 8
    Status[16] = FiFo_Get(&GetData[6]);  // should succeed, 7 8
    Status[17] = FiFo_Get(&GetData[7]);  // should succeed, 8
    Status[18] = FiFo_Get(&GetData[8]);  // should succeed, empty
    Status[19] = FiFo_Get(&GetData[9]);  // should fail,    empty
  }
}

void SysTick_Init(void){
	
	NVIC_ST_CTRL_R =0; 																							//Disable SysTick during setup 
	NVIC_ST_RELOAD_R= 0x001E8480; 																		//reload value, this should be 40 HZ
	NVIC_ST_CURRENT_R =0; 																					// clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000;  //Priority 1
	NVIC_ST_CTRL_R = 0x0007; 	
	//EnableInterrupts();	
	// Enable Systick with core clock and interrupts. Not sure if 0x0007 is right or if it should be different for 6-bit DAC
}


