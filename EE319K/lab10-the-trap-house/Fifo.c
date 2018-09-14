// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 4/10/2017 
// Student names: Michael Hernandez and Arkan Abuyazid
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "ST7735.h"
#include "Fifo.h"
#include "tm4c123gh6pm.h"
#define SIZE 9
//lit
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
  uint8_t static PutI1; 
	uint8_t static GetI1;
	uint8_t static Counter1;
	int32_t static FIFO1[5];
  uint8_t static PutI2; 
	uint8_t static GetI2;
	uint8_t static Counter2;
	int32_t static FIFO2[5];
  uint8_t static PutI7; 
	uint8_t static GetI7;
	uint8_t static Counter7;
	int32_t static FIFO7[SIZE];
	long sr;
// --UUU-- Declare state variables for FiFo
//        size, buffer, put and get indexes

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init1() {
	PutI1  = GetI1 = Counter1 = 0;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put1(char data) {
	sr = StartCritical();
	if(Counter1 == 5){
		EndCritical(sr);
		return 0;
	}
	if(Counter1 == 0){
		PutI1 = 0;
	}
	FIFO1[PutI1] = data;
	PutI1++;
	Counter1++;
	EndCritical(sr);
	return 1;
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get1(char *datapt){
	sr = StartCritical();
	if(Counter1 == 0){
		GetI1 = 0;
		EndCritical(sr);
		return 0;
	}
	if(Counter1 == 5){
		
	}
	*datapt = FIFO1[GetI1];
	GetI1++;
	Counter1--;
	EndCritical(sr);
	return 1;
}

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init2() {
	PutI2  = GetI2 = Counter2 = 0;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put2(char data) {
	sr = StartCritical();
	if(Counter2 == 5){
		EndCritical(sr);
		return 0;
	}
	if(Counter2 == 0){
		PutI2 = 0;
	}
	FIFO2[PutI2] = data;
	PutI2++;
	Counter2++;
	EndCritical(sr);
	return 1;
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get2(char *datapt){
	sr = StartCritical();
	if(Counter2 == 0){
		GetI2 = 0;
		EndCritical(sr);
		return 0;
	}
	if(Counter2 == 5){
		
	}
	*datapt = FIFO2[GetI2];
	GetI2++;
	Counter2--;
	EndCritical(sr);
	return 1;
}

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init7() {
	PutI7  = GetI7 = Counter7 = 0;
}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put7(char data) {
	sr = StartCritical();
	if(Counter7 == SIZE){
		EndCritical(sr);
		return 0;
	}
	if(Counter7 == 0){
		PutI7 = 0;
	}
	FIFO7[PutI7] = data;
	PutI7++;
	Counter7++;
	EndCritical(sr);
	return 1;
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get7(char *datapt){
	sr = StartCritical();
	if(Counter7 == 0){
		GetI7 = 0;
		EndCritical(sr);
		return 0;
	}
	if(Counter7 == SIZE){
		
	}
	*datapt = FIFO7[GetI7];
	GetI7++;
	Counter7--;
	EndCritical(sr);
	return 1;
}
