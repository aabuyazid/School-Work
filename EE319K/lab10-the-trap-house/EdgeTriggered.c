#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Fifo.h"
#include "UART.h"
#include "EdgeTriggered.h"
#define PF1       (*((volatile uint32_t *)0x40025008))
extern uint8_t PORTFMailbox;

//------------PortF_Init------------
// Turn on clock and set PF0 as input
// Input: none
// Output: none
void PortF_Init(void){
  SYSCTL_RCGCGPIO_R|= 0x20;  
	uint8_t delay;
	delay |= 10;
	//uint8_t Counter1=0;
	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	GPIO_PORTF_CR_R = 0x1F;
	GPIO_PORTF_DIR_R &= ~0x11;
	GPIO_PORTF_PUR_R |= 0x11;
  GPIO_PORTF_AFSEL_R &= ~0x11;         
  GPIO_PORTF_DEN_R |= 0x11;   
  GPIO_PORTF_PCTL_R &= ~0x000F0000; // How to configure PF0??
  GPIO_PORTF_AMSEL_R = 0;
	GPIO_PORTF_IS_R &= ~0x10;     // (d) PF4 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x10;    //     PF4 is not both edges
  GPIO_PORTF_IEV_R &= 0x10;    //     PF4 falling edge event
  GPIO_PORTF_ICR_R = 0x10;      // (e) clear flag4
  GPIO_PORTF_IM_R |= 0x10;      // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00A00000; // (g) priority 5
  NVIC_EN0_R = 0x40000000;      // (h) enable interrupt 30 in NVIC
	PF1=0x02;
}	

//------------GPIOPortF_Handler------------
// If button press, sends MC result of button press, may need switch debounce
// Input: none
// Output: none
/*void GPIOPortF_Handler(void){
	char buttonP2;
	uint32_t i;
	if( (GPIO_PORTF_DATA_R& 0x10)==1){}
		for(i=0;i<727240;i++){}  
  //if (Counter1 ==0){
	PF1^= 0x02;
	buttonP2 = 0x02;
	UART1_OutChar(buttonP2);
	buttonP2 = 0x40;
	UART1_OutChar(buttonP2);
	buttonP2 = 0x35;
	UART1_OutChar(buttonP2);
  buttonP2 = 0x03;	
	UART1_OutChar(buttonP2); //these were uncommented
  GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag1 or flag4??
		//Counter =1;
	}
*/
// GPIO_Handler for MC1
	
	void GPIOPortF_Handler(void){
	uint32_t i;
	if( (GPIO_PORTF_DATA_R& 0x10)==1){}
  PORTFMailbox = 0x35;
  GPIO_PORTF_ICR_R = 0x10;      // acknowledge flag1 or flag4??
		//Counter =1;
	}
