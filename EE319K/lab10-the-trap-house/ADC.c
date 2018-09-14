// put implementations for functions, explain how it works
// Michael Hernandez and Arkan Abuyazid
#include <stdint.h>
#include "ADC.h"
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
	volatile uint32_t delay=0;
  SYSCTL_RCGCGPIO_R |= 0x10;
  while((SYSCTL_RCGCGPIO_R&0x10) == 0x0){};
  GPIO_PORTE_DIR_R &= ~0x24;      // 2) make PE2 and PE5 input
  GPIO_PORTE_AFSEL_R |= 0x24;     // 3) enable alternate function on PE2 and PE5
  GPIO_PORTE_DEN_R &= ~0x24;      // 4) disable digital I/O on PE4
  GPIO_PORTE_AMSEL_R |= 0x24;     // 5) enable analog functionality on PE2 and PE5
  SYSCTL_RCGCADC_R |= 0x01;       // 6) activate ADC0 
  delay = SYSCTL_RCGCADC_R;       // extra time for clock to stabilize
  delay = SYSCTL_RCGCADC_R;         
  delay = SYSCTL_RCGCADC_R;         
  delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;	
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;	
	delay = SYSCTL_RCGCADC_R;	
	delay = SYSCTL_RCGCADC_R;	
	delay = SYSCTL_RCGCADC_R;	
	delay = SYSCTL_RCGCADC_R;
	delay = SYSCTL_RCGCADC_R;	
	delay = SYSCTL_RCGCADC_R;
  ADC0_PC_R &= ~0xF;              // 7) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 8) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 9) disable sample sequencer 2
  ADC0_EMUX_R &= ~0xF000;         // 10) seq2 is software trigger
	ADC0_SSMUX2_R &= ~0x00FF;       // 11) clear SS3 field
  ADC0_SSMUX2_R |= 0x0080;          //    set channel, CHECK THIS LINE
	ADC0_SSMUX2_R |= 0x0001;          //    set channel, CHECK THIS LINE
	ADC0_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC0_IM_R &= ~0x0004;           // 13) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0004;         // 14) enable sample sequencer 3
	ADC0_SAC_R = 0x04;

}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
CoordPot ADC_In( void ){  
	ADC0_PSSI_R = 0x0004;
	while((ADC0_RIS_R & 0x0004) == 0){	
	}
	CoordPot temp;
	temp.y = ADC0_SSFIFO2_R & 0x0FFF;
	temp.x = ADC0_SSFIFO2_R & 0x0FFF;	
	ADC0_ISC_R = 0x0004;
	
	return temp;
}
