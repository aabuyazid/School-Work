// FiFo.c
// Runs on LM4F120/TM4C123
// Provide functions that implement the Software FiFo Buffer
// Last Modified: 4/10/2017 
// Student names: Michael Hernandez and Arkan Abuyazid
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "ST7735.h"
#define SIZE 9

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void PRINTIT(char duh);
  uint32_t static PutI; //MUST RE-WRITE
	uint32_t static GetI;
	uint32_t static Counter;
	int32_t static FIFO[SIZE];
	uint32_t t=0;
	long sr;
// --UUU-- Declare state variables for FiFo
//        size, buffer, put and get indexes

// *********** FiFo_Init**********
// Initializes a software FIFO of a
// fixed size and sets up indexes for
// put and get operations
void FiFo_Init() {
	//sr = StartCritical();
	PutI  = GetI = Counter = 0;
	//EndCritical(sr);
	

// --UUU-- Complete this

}

// *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(char data) {
	sr = StartCritical();
	if(Counter == SIZE){
		EndCritical(sr);
		return 0;
	}
	if(Counter == 0){
		PutI = 0;
	}
	FIFO[PutI] = data;
	PutI++;
	Counter++;
	EndCritical(sr);
	return 1;
}

// *********** FiFo_Get**********
// Gets an element from the FIFO
// Input: Pointer to a character that will get the character read from the buffer
// Output: 1 for success and 0 for failure
//         failure is when the buffer is empty
uint32_t FiFo_Get(char *datapt){
	sr = StartCritical();
	if(Counter == 0){
		GetI = 0;
		EndCritical(sr);
		return 0;
	}
	if(Counter == SIZE){
		
	}
	*datapt = FIFO[GetI];
	GetI++;
	Counter--;
	EndCritical(sr);
	return 1;
}

/* *********** FiFo_Put**********
// Adds an element to the FIFO
// Input: Character to be inserted
// Output: 1 for success and 0 for failure
//         failure is when the buffer is full
uint32_t FiFo_Put(char data) {
if(((PutI+1)% SIZE) == GetI){
return 0;}
FIFO[PutI] =data;
//if (data == 0x03){
	//for (t=1; t<6; t++){
//(FIFO[t]);
	//}
	//	ST7735_SetCursor(0,0);
//}
PutI = (PutI+1)% SIZE;
return 1;
}

uint32_t FiFo_Get(char *datapt){
if (PutI ==GetI){return 0;}
*datapt = FIFO[GetI];
GetI = (GetI +1)% SIZE;
return 1;

}

void PRINTIT(char duh){
ST7735_OutChar(duh);
}*/
