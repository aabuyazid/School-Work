// UART1.c
// Runs on LM4F120/TM4C123
// Use UART1 to implement bidirectional data transfer to and from 
// another microcontroller in Lab 9.  This time, interrupts and FIFOs
// are used.
// Daniel Valvano
// November 17, 2014
// Modified by EE345L students Charlie Gough && Matt Hawk
// Modified by EE345M students Agustinus Darmawan && Mingjie Qiu

/* Lab solution, Do not post
 http://users.ece.utexas.edu/~valvano/
*/

// U1Rx (VCP receive) connected to PC4
// U1Tx (VCP transmit) connected to PC5
#include <stdint.h>
#include "FiFo.h"
#include "UART.h"
#include "tm4c123gh6pm.h"
#define  PF2       (*((volatile uint32_t *)0x40025010))
#define UART_CTL_TXE 0x00000100 // UART Transmit FIFO enable
#define UART_CTL_RXE 0x00000200 // UART 
void   RxFifo_Init(void);                        // initialize empty FIFOs
void   TxFifo_Init(void);
//void EnableInterrupts(void);  // Enable interrupts
uint32_t DataLost;
uint32_t RxCounter =0;
uint32_t work =0;
// Initialize UART1
// Baud rate is 115200 bits/sec
// Make sure to turn ON UART1 Receiver Interrupt (Interrupt 6 in NVIC)
// Write UART1_Handler
void UART_Init(void){
	uint32_t delayfunct=0;
	SYSCTL_RCGCUART_R |= 0x02;            // activate UART1; Is this UART1?
	for (delayfunct =0; delayfunct <100; delayfunct++){}
  SYSCTL_RCGCGPIO_R |= 0x04;            // activate port C
	for (delayfunct =0; delayfunct <100; delayfunct++){}
	FiFo_Init();
  UART1_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART1_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115,200)) = int(27.1267)
  UART1_FBRD_R = 26;                     // FBRD = int(0.1267 * 64 + 0.5) = 8

		// 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART1_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
  UART1_IFLS_R &= ~0x3F;                // clear TX and RX interrupt FIFO level fields
                                        // configure interrupt for TX FIFO <= 1/8 full
                                        // configure interrupt for RX FIFO >= 1/8 full
  //UART1_IFLS_R += (UART_IFLS_TX1_8|UART_IFLS_RX1_8);// I don't need to arm anything else?
	UART1_IFLS_R|= 0x2;
                                        // enable TX and RX FIFO interrupts and RX time-out interrupt
  //UART1_IM_R |= (UART_IM_RXIM|UART_IM_TXIM|UART_IM_RTIM);
	UART1_IM_R |= 0x10;
	//	  UART1_LCRH_R = 0x0070; 
	//UART1_CTL_R |= 0x0301;
		UART1_CTL_R |= (UART_CTL_UARTEN|UART_CTL_TXE|UART_CTL_RXE);
  //UART1_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTC_AFSEL_R |= 0x30;           // enable alt funct on PC4-5
  
                                        // configure PC4-5 as UART
  GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000; //?? Do i shift bits??
  GPIO_PORTC_AMSEL_R &= ~0x30;               // disable analog functionality on PC
	GPIO_PORTC_DEN_R |= 0x30;             // enable digital I/O on PC4-5
                                        // UART0=priority 2????
  NVIC_PRI1_R = (NVIC_PRI1_R&0xFF00FFFF)|0x00E00000; // bits 21-23
  NVIC_EN0_R |= NVIC_EN0_INT6;           // enable interrupt 6 in NVIC
   // --UUU-- complete with your code
	 UART1_ICR_R |= 0x10;
}

// input ASCII character from UART
// spin if RxFifo is empty
char UART_InChar(void){// THESE ARE WRONG, WRONG FIFOs
	while((UART1_FR_R&0x0010) != 0){};      // wait until RXFE is 0
  return((char)(UART1_DR_R&0xFF));
  // --UUU-- remove this, replace with real code
}
//------------UART1_OutChar------------
// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
// Output: none
void UART_OutChar(char data){
	while((UART1_FR_R&0x0020) != 0){};      // wait until TXFF is 0
  UART1_DR_R = data;
		return;
  // --UUU-- complete with your code
}

// hardware RX FIFO goes from 7 to 8 or more items
// UART receiver Interrupt is triggered; This is the ISR
void UART1_Handler(void){
	uint32_t i=0;
	char byte1;
	PF2 ^= 0x04;
	PF2 ^= 0x04;
	for(i=0; i<8; i++){
		byte1= UART_InChar();
    FiFo_Put(byte1);	
	}
	RxCounter++;
	UART1_ICR_R = 0x10;
	PF2 ^= 0x04; 
	work =1;
	return;
  // --UUU-- complete with your code
}
