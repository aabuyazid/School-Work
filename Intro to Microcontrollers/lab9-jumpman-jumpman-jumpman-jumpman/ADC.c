// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 3/6/2015 
// Student names: Michael Hernandez & Arkan Abuyazid
// Last modification date: 4/12/17

#include <stdint.h>
#include "tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ 
	volatile uint32_t delay=0;
  SYSCTL_RCGCGPIO_R |= 0x10;
  while((SYSCTL_RCGCGPIO_R&0x10) == 0x0){};
  //delay = SYSCTL_RCGCGPIO_R;      //   allow time for clock to stabilize
  //delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE3 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog functionality on PE2
  SYSCTL_RCGCADC_R |= 0x01;   // 7) activate ADC0 
	//while((SYSCTL_RCGCADC_R&0x0001) == 0x0){};    // good code, but not yet implemented in simulator
  delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
  delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
  delay = SYSCTL_RCGCADC_R;         // extra time for clock to stabilize
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
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;       // 11) clear SS3 field
  ADC0_SSMUX3_R += 1;             //    set channel
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
	ADC0_SAC_R = 0x04;

}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	uint32_t result;
	ADC0_PSSI_R = 0x0008;
	while((ADC0_RIS_R & 0x0008) == 0){
		
	}
	result = ADC0_SSFIFO3_R & 0x0FFF;
	ADC0_ISC_R = 0x0008;
  return result; // remove this, replace with real code
}


