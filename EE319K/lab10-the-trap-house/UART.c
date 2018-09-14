
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Fifo.h"
#include "UART.h"
#define UART_CTL_TXE 0x00000100 // UART Transmit FIFO enable
#define UART_CTL_RXE 0x00000200 // UART 
void   RxFifo_Init(void);                        // initialize empty FIFOs
void   TxFifo_Init(void);
extern uint8_t UARTMailboxDir;
extern uint8_t UARTMailboxBut;
uint32_t work1 =0;
uint32_t work2 =0;
uint32_t work7 =0;
uint8_t direction;
#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
//------------UART_Init------------
// Initialize PC4/PC5, PE0/PE1, PD6/PD7...
// Input: none
// Output: none
void UART_Init(void){ 
  uint32_t delayfunct=0;
	SYSCTL_RCGCUART_R |= 0x86;            // activate UART1, UART2, UART7;
	while ((SYSCTL_RCGCUART_R & 0x00000002)==0){}
	for (delayfunct =0; delayfunct <100; delayfunct++){}
		delayfunct |=10;
		delayfunct |=10;
		delayfunct |=10;
		delayfunct |=10;
		delayfunct |=10;
		delayfunct |=10;
		delayfunct |=10;
		delayfunct |=10;
		delayfunct |=10;
		delayfunct |=10;
  SYSCTL_RCGCGPIO_R |= 0x04;            // activate port C AND port D
	for (delayfunct =0; delayfunct <100; delayfunct++){}
	FiFo_Init1();
	FiFo_Init2();
	FiFo_Init7();
  UART1_CTL_R &= ~UART_CTL_UARTEN;      // disable UART1
  UART1_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115,200)) = int(27.1267)
  UART1_FBRD_R = 26;                     // FBRD = int(0.1267 * 64 + 0.5) = 8
  UART1_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN); //sending less than 8 bits?
  UART1_IFLS_R &= ~0x3F;                // clear TX and RX interrupt FIFO level fields
                                        // configure interrupt for TX FIFO <= 1/4 full
                                        // configure interrupt for RX FIFO >= 1/4 full
  UART1_IFLS_R|= 0x1;
                                        // enable TX and RX FIFO interrupts and RX time-out interrupt
  UART1_IM_R |= 0x10;
	UART1_CTL_R |= (UART_CTL_UARTEN|UART_CTL_TXE|UART_CTL_RXE);
  GPIO_PORTC_AFSEL_R |= 0x30;           // enable alt funct on PC4-5  
                                        // configure PC4-5 as UART
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000; //?? Do i shift bits??
  GPIO_PORTC_AMSEL_R &= ~0x30;               // disable analog functionality on PC
	GPIO_PORTC_DEN_R |= 0x30;             // enable digital I/O on PC4-5
                                        // UART0=priority 2????
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFF00FFFF)|0x00E00000; // bits 21-23
  NVIC_EN0_R |= NVIC_EN0_INT6;           // enable interrupt 6 in NVIC
	UART1_ICR_R |= 0x10;
	
	
	
	
//UART2 INIT	
/*
	UART2_CTL_R &= ~UART_CTL_UARTEN;      // disable UART2
  UART2_IBRD_R = 43;                    
  UART2_FBRD_R = 26;                     
  UART2_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART2_IFLS_R &= ~0x3F;                
  UART2_IFLS_R|= 0x1;   // 1/4 full
  UART2_IM_R |= 0x10;
	UART2_CTL_R |= (UART_CTL_UARTEN|UART_CTL_TXE|UART_CTL_RXE);
	GPIO_PORTD_LOCK_R = 0x4C4F434B; //HOW DO I UNLOCK PORTD
	GPIO_PORTD_CR_R = 0x1F;
  GPIO_PORTD_AFSEL_R |= 0xC0;           // enable alt funct on PD6-7
                                        // configure PC6-7 as UART
  GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&0x00FFFFFF)+0x22000000; 
  GPIO_PORTD_AMSEL_R &= ~0xC0;               // disable analog functionality on PC
	GPIO_PORTD_DEN_R |= 0xC0;             // enable digital I/O on PD6-7
                                        // UART2=priority 2??
  NVIC_PRI8_R = (NVIC_PRI8_R&0xFFFF0FFF)|0x0000E000; // bits 21-23 PRIORITY 7
  NVIC_EN1_R |= 0x02;// enable interrupt 33 in NVIC
	UART2_ICR_R |= 0x10;
*/
	
	
	
	
//UART7 INIT	
	
/*	UART7_CTL_R &= ~UART_CTL_UARTEN;      // disable UART7
  UART7_IBRD_R = 43;                    
  UART7_FBRD_R = 26;                     
  UART7_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN); //8 BITS AND FIFO ENABLE
  UART7_IFLS_R &= ~0x3F;      //CLEARS FIFOs          
  UART7_IFLS_R|= 0x1;         //read four
  UART7_IM_R |= 0x10;         //interrupt sent  to controller
	UART7_CTL_R |= (UART_CTL_UARTEN|UART_CTL_TXE|UART_CTL_RXE);
  GPIO_PORTE_AFSEL_R |= 0x03;           // enable alt funct on PE0-1
                                        // configure PE0-1 as UART
  GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFFFFF00)+0x00000022; 
  GPIO_PORTE_AMSEL_R &= ~0x03;               // disable analog functionality on PC
	GPIO_PORTE_DEN_R |= 0x03;             // enable digital I/O on PE0-1
                                        // UART2=priority 2??
  NVIC_PRI15_R = (NVIC_PRI15_R&0x00FFFFFF)|0xE0000000; // bits 21-23 PRIORITY 7
  NVIC_EN1_R |= 0x80000000;           // enable interrupt 63 in NVIC
	UART7_ICR_R |= 0x10;
	*///WHAT AM I SUPPOSED TO CHANGE IN THE CODE?
}

char UART1_InChar(void){
	while((UART1_FR_R&0x0010) != 0){};     
  return((char)(UART1_DR_R&0xFF));
}

void UART1_OutChar(char data){
	while((UART1_FR_R&0x0020) != 0){};      // wait until TXFF is 0
  UART1_DR_R = data;
		return;
}
/*char UART2_InChar(void){
	while((UART2_FR_R&0x0010) != 0){};      // wait until RXFE is 0
  return((char)(UART2_DR_R&0xFF));
}

void UART2_OutChar(char data){
	while((UART2_FR_R&0x0020) != 0){};      // wait until TXFF is 0
  UART2_DR_R = data;
		return;
}*/

char UART7_InChar(void){
	while((UART7_FR_R&0x0010) != 0){};      // wait until RXFE is 0
  return((char)(UART7_DR_R&0xFF));
}

void UART7_OutChar(char data){
	while((UART7_FR_R&0x0020) != 0){};      // wait until TXFF is 0
  UART7_DR_R = data;
		return;
}

// hardware RX FIFO goes from 7 to 8 or more items
// UART receiver Interrupt is triggered; This is the ISR
void UART1_Handler(void){
	uint32_t i=0;
	char byte1;
	byte1= UART1_InChar();
	for(i=0; i<3; i++){
			byte1= UART1_InChar();
	  if(byte1 == 0x40){
		//PF1 ^= 0x02;
			UARTMailboxBut = byte1;
		}
		//if (byte1 ==0x41){
		//PF2 ^=0x04;
			//UARTMailboxBut = byte1;
		//}
		if (byte1 == 0x30 || byte1 == 0x31 ||byte1 ==  0x32 ||byte1 == 0x33|| byte1 == 0x34){
		 UARTMailboxDir = byte1; // In main loop check to see what this corresponds to in which direction
		}
    FiFo_Put1(byte1);	
	}
	UART1_ICR_R = 0x10; 
//	bulletcheck =1; //In main loop, set this equal to zero after acknowledged.
	return;
}


/*void UART2_Handler(void){
	uint32_t i=0;
	char byte1;
	for(i=0; i<4; i++){
		byte1= UART2_InChar();
		
    FiFo_Put2(byte1);	
	}
	UART2_ICR_R = 0x10; 
	work2 =1;
	return;
}*/

void UART7_Handler(void){
	uint32_t i=0;
	char byte1;
	for(i=0; i<8; i++){
		byte1= UART7_InChar();
		if (byte1 == 0x30 || byte1 == 0x31 ||byte1 ==  0x32 ||byte1 == 0x33|| byte1 == 0x34){
	//	 direction = byte1; // In main loop check to see what this corresponds to in which direction
		}
    FiFo_Put7(byte1);	
	}
	UART7_ICR_R = 0x10; 
	work7 =1;
	return;
}
