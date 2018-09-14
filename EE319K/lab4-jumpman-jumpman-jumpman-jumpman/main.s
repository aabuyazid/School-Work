;****************** main.s ***************
; Program written by: ***Michael Hernandez and Arkan Abuyazid
; Date Created: 2/14/2017
; Last Modified: 3/01/2017
; Brief description of the program
;   The LED toggles at 8 Hz and a varying duty-cycle
;   Repeat the functionality from Lab2-3 but now we want you to 
;   insert debugging instruments which gather data (state and timing)
;   to verify that the system is functioning as expected.
; Hardware connections (External: One button and one LED)
;  PE1 is Button input  (1 means pressed, 0 means not pressed)
;  PE0 is LED output (1 activates external LED on protoboard)
;  PF2 is Blue LED on Launchpad used as a heartbeat
; Instrumentation data to be gathered is as follows:
; After Button(PE1) press collect one state and time entry. 
; After Buttin(PE1) release, collect 7 state and
; time entries on each change in state of the LED(PE0): 
; An entry is one 8-bit entry in the Data Buffer and one 
; 32-bit entry in the Time Buffer
;  The Data Buffer entry (byte) content has:
;    Lower nibble is state of LED (PE0)
;    Higher nibble is state of Button (PE1)
;  The Time Buffer entry (32-bit) has:
;    24-bit value of the SysTick's Current register (NVIC_ST_CURRENT_R)
; Note: The size of both buffers is 50 entries. Once you fill these
;       entries you should stop collecting data
; The heartbeat is an indicator of the running of the program. 
; On each iteration of the main loop of your program toggle the 
; LED to indicate that your code(system) is live (not stuck or dead).

GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_AFSEL_R EQU 0x40024420
GPIO_PORTE_DEN_R   EQU 0x4002451C

GPIO_PORTF_DATA_R  EQU 0x400253FC
GPIO_PORTF_DIR_R   EQU 0x40025400
GPIO_PORTF_AFSEL_R EQU 0x40025420
GPIO_PORTF_PUR_R   EQU 0x40025510
GPIO_PORTF_DEN_R   EQU 0x4002551C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
ENTRIES 		   EQU 50
DATA_RESET		   EQU 0xFF
TIME_RESET		   EQU 0xFFFFFFFF
TERM               EQU 0x00030B00	; one millisecond 2867C (hardware lab 2-3),  30B00 (software),  28A9C (hardware for lab 4)
SHORT              EQU 0x00000FFF
BIG                EQU 0x00300000
; RAM Area
           AREA    DATA, ALIGN=2
			              

	        
;-UUU-Declare  and allocate space for your Buffers 
;    and any variables (like pointers and counters) here

DataBuffer 	    SPACE ENTRIES * 1
TimeBuffer 	    SPACE ENTRIES * 4
DataPt 			SPACE 4
TimePt 			SPACE 4
CYCLE 			SPACE 1
NEntries 		SPACE 1
Switch_Hit      SPACE 1	
DutyCounter     SPACE 1
			    ALIGN 
; ROM Area

       IMPORT  TExaS_Init
	   IMPORT  SysTick_Init
		   
;-UUU-Import routine(s) from other assembly files (like SysTick.s) here

       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT  Start

Start
      BL  			TExaS_Init 		; voltmeter, scope on PD3
	  BL            SysTick_Init
      CPSIE  		I    			; TExaS voltmeter, scope runs on interrupts
	  BL 			Initialization;
	  BL    		Debug_Init;
	  LDR   R1, =DutyCounter; 
	  AND 	R0, R0, #0;
	  STRB  R0, [R1];
	  LDR  	R1, =NEntries;
      STRB  R0, [R1];	  
loop  
      BL 			HIT;
      BL 			LED_ON;
	  BL 			Debug_Capture;
	  BL  			DELAY_ON;
	  BL 			LED_OFF;
	  BL 			Debug_Capture;
	  BL 			DELAY_OFF;
	  BL 			HeartBeat;
      B    			loop;


      



;------------Debug_Init------------
; Resets both buffers, initializes the points 
; of said buffers, and activates SysTick Timer
; Input: None
; Output: None
		
Debug_Init
		
DPointer RN 0
DElement RN 1
Count RN 2
	  LDR		DPointer, =DataPt				; Stores address of first data element
	  LDR		R1, =DataBuffer					; to DataPt
	  STR		R1, [DPointer]
	  LDR		DPointer, [DPointer]
	  MOV		DElement, #0xFFF
	  MOV 	Count, #0
Data_Clear
	  STRB	DElement, [DPointer, Count]
	  ADD		Count, Count, #1
	  CMP 	Count, #ENTRIES
	  BHS		Time_Init
	  B			Data_Clear
Time_Init
TPointer RN 0
TElement RN 1
	  LDR 		TPointer, =TimePt
	  LDR		R1, =TimeBuffer
	  STR		R1, [TPointer]
	  LDR		TPointer, [TPointer]
	  LDR		TElement, =0xFFFFFFFF
	  MOV		Count, #0
Time_Clear
	  STR		TElement, [TPointer, Count]
	  ADD		Count, Count, #4
	  CMP 	Count, #ENTRIES*4
	  BHS		Owari
      B			Time_Clear
Owari		
      PUSH {R0, LR}						; ADDED THIS PUSH AND POP BECAUSE CODE BECAME STUCK IN ENDLESS LOOP
	  BL		SysTick_Init			; Initializes SysTick
	  POP  {R0, LR};
	  BX 		LR




Initialization	
      LDR 	R1, =SYSCTL_RCGCGPIO_R		; TExaS_Init sets bus clock at 80 MHz 
      LDR 	R0, [R1]					;Initialization 
      ORR 	R0,  R0, #0x30;  
      STR 	R0, [R1]; 
      NOP; 
      NOP; 
      NOP; 
      NOP; 
      LDR 	R1, =GPIO_PORTE_DIR_R; 
      LDR 	R0, [R1]; 
      AND 	R0,  R0, #0xFD; 
      ORR 	R0,  R0, #0x01				; PE1 input and PE0 output 
      STR 	R0, [R1]; 
      LDR 	R1, =GPIO_PORTE_AFSEL_R; 
      LDR 	R0, [R1]; 
      AND 	R0,  R0, #0x00;  
      STR 	R0, [R1]; 
      LDR 	R1, =GPIO_PORTE_DEN_R; 
      LDR 	R0, [R1]; 
      ORR 	R0,  R0, #0x03				; enables just PE0 and PE1 
      STR 	R0, [R1]; 
      LDR 	R1, =GPIO_PORTF_DIR_R; 
      LDR 	R0, [R1]; 
	  AND   R0,  R0, #0x00				; PF4 INPUT
	  ORR   R0,  R0, #0x04				; PF2 OUTPUT
      STR 	R0, [R1]; 
      LDR 	R1, =GPIO_PORTF_AFSEL_R; 
      LDR 	R0, [R1]; 
      AND 	R0,  R0, #0x0000; 
      STR 	R0, [R1]; 
      LDR 	R1, =GPIO_PORTF_PUR_R; 
      LDR 	R0, [R1]; 
      ORR 	R0,  R0, #0x10; 
      STR 	R0, [R1]; 
      LDR 	R1, =GPIO_PORTF_DEN_R; 
      LDR 	R0, [R1]; 
      ORR 	R0,  R0, #0x14; 
      STR 	R0, [R1]					;END of initialization 
										; TExaS_Init sets bus clock at 80 MHz

      AND 	R0, #0x0000; 
      ADD 	R0,  R0, #16; 
      LDR 	R1, =CYCLE; 
      STRB 	R0, [R1]					;code to store 20% to duty cycle before running loop 
	  LDR   R0, =Switch_Hit;
	  AND   R1, R1, #0x00;
	  STRB  R1, [R0];
	  BX    LR;
	  
	  
	  ;;------------HIT------------
; Updates duty cycle 
; 
; Input: None
; Output: None
	  
HIT     								; implement debug_Capture here 
	  PUSH   {R0-R2, R3};
	  LDR    R0, =Switch_Hit;
	  LDRB   R1, [R0];
	  CMP    R1, #0;
	  BNE   			Still_Pressed; 
	  LDR 	 R0, =GPIO_PORTE_DATA_R		; check for switch release 
      LDRSB  R1, [R0];
	  AND    R1, R1, #0x02;
	  CMP  	 R1, #0x02;
	  BEQ   			Store;
	  POP 	 {R0-R2, R3};
	  BX    			 LR;
Store
      LDR 	R0, =Switch_Hit
	  AND 	R1, R1, #0;
	  ADD 	R1, R1, #1;
	  STRB  R1, [R0];
	  POP  {R0-R2, R3};
	  BX    			LR;
Still_Pressed
      LDR 	R0, =GPIO_PORTE_DATA_R;
	  LDR 	R1, [R0];
	  AND 	R1,  R1, #0x02;
	  CMP 	R1, #0x02;
	  BNE  				 Update;
	  POP 	{R0-R2, R3};
	  BX    			 LR;
Update	  
      LDR   R0, =Switch_Hit;
	  AND   R1, R1, #0x00;
	  STRB  R1, [R0];
	  LDR 	R0, =CYCLE				; Duty Cycle subroutine 
      LDRB 	R1, [R0]; 
      ADDS 	R1, #16					; Duty cycle to next level 
      CMP 	R1, #80					; 0X60 
      BNE 			      Valid;  
	  
      STRB 	R1, [R0]; 
      SUBS 	R1, #80; 
      B 				  DONE; 
Valid 
      CMP 	R1, #96; 
      BEQ 		RESET; 
      STRB 	R1, [R0]; 
      B 				  DONE;		 ; GOES TO DELAY SUBROUTINE 
RESET 
      SUBS 	R1, #96; 
      STRB 	R1, [R0];
DONE
      POP {R0-R2, R3};
      BX 		LR; 
	  
	  ;------------Debug_Capture------------
; Updates buffers and  pointers
; 
; Input: None
; Output: None
Debug_Capture 
      PUSH {R0-R11, R12, LR} 								; %= 0.00196 calculated as  ((100) (.000002450))/.125
      LDR   R1, 	=NEntries;
	  LDRB  R0, [R1];
	  CMP   R0, 	#50;
	  BEQ 			Full;
	  LDR   R11, =Switch_Hit;
	  LDRB  R10, [R11];
	  CMP   R10, #0;
	  BNE  			Next_Case;
	  LDR   R1, =DutyCounter;
	  LDRB  R0, [R1];
	  CMP   R0, #0;
	  BEQ 			skipped;
	  CMP   R0, #7;
	  BNE 			Run
	  AND   R0, R0, #0x0000			; this line may cause problems
	  STR	R0, [R1];
	  B     Clear;
	  
Run
      ADD   R0, R0, #1;
	  STRB  R0, [R1];
Clear
	  LDR 	R1, 	=GPIO_PORTE_DATA_R;
	  LDR 	R0, [R1];
	  LDR 	R3, 	=0xE000E018 ;
	  LDR 	R2, [R3];
	  AND   R4,  R0, #0x02			; Port E bit 1
	  AND   R5,  R0, #0X01		    ; Port E bit 0
	  LSL   R4,  R4, #3;
	  ORR   R4,  R4, R5;
	  LDR   R6, 	=DataPt;
	  LDR   R7,  [R6];
	  STRB  R4,  [R7]; 
	  ADD   R7,   R7, #1;
	  STR   R7,  [R6];
	  LDR   R8, 	=TimePt;
	  LDR   R9,  [R8];
	  STR   R2,  [R9];
	  ADD   R9,   R9, #4;
	  STR   R9,  [R8]				; Assumes NVIC_ST_CURRENT_R is the time information
	  
	  LDR   R1,   =NEntries;
	  LDRB  R0, [R1];
	  ADD   R0, R0, #1
	  STRB  R0,  [R1]; 
	  POP  {R0-R11, R12, PC}
Full  
      POP  {R0-R11, R12, PC}
Next_Case
	  LDR R1, =DutyCounter;
	  LDRB  R0, [R1];
	  CMP   R0, #0;
      BEQ   Run
skipped	  
      POP   {R0-R11, R12, PC}  

	;------------HeartBeat------------
; Toggles PF2 Blue LED on and  off
; 
; Input: PF2
; Output: PF2
HeartBeat
      PUSH { R0-R3}
	  LDR   R1, 	=GPIO_PORTF_DATA_R ;
	  LDR 	R0, [R1]; 
	  EOR 	R0,  R0, #0x04;
	  STR 	R0, [R1]; 
	  POP 	{R0-R3};
	  BX    LR;
	  
	  
	  
;-----------LED_ON----------
; Turns on LED
;
; Input: none
; Output: None
LED_ON
	  PUSH  {R0, R3}
	  LDR 	R0, =GPIO_PORTE_DATA_R	;Turn on LED  
	  LDR 	R3, [R0]; 
	  ORR 	R3, #0X01; 
	  STR 	R3, [R0];
      POP   {R0, R3}
	  BX 	LR
;-----------LED_OFF----------
; Turns off LED
;
; Input: none
; Output: None
LED_OFF
	  PUSH  {R0, R3}
	  LDR 	R0, =GPIO_PORTE_DATA_R	;Turn on LED  
	  LDR 	R3, [R0]; 
	  AND 	R3, #0XFE; 
	  STR 	R3, [R0];
	  POP   {R0, R3}
	  BX 	LR
	  
	  
	  
	  
;---------DELAY_ON-------
;
;
DELAY_ON
      PUSH {R0-R3}
      LDR 	R0, =CYCLE				;Delay subroutine  
      LDRSB R1, [R0]; 
      CMP 	R1, #0;
      BEQ    LEAVE;
REPEAT2
      LDR 	R2, =TERM;	  
REPEAT

      SUBS  R2, #1					; Millisecond load 
      CMP 	R2, #0;  
      BNE 		REPEAT; 
      SUBS 	R1, #8;  
      CMP 	R1, #0					; on for duty cycle 
      BNE 		REPEAT2;
      
LEAVE
      POP {R0-R3}
      BX LR
	  
	  
	  
	  
;----------DELAY_OFF-------
;
;
;
DELAY_OFF
      PUSH {R0-R3}
      LDR 	R0, =CYCLE; 
      LDRB 	R1, [R0]; 
      RSBS 	R1,  R1, #80;
      BEQ  	  LEAVE3;
     

      
REPEAT4
      LDR 	R2, =TERM; 
REPEAT5
   
      SUBS 	R2, #1										; off for (duty cycle)! 
      CMP 	R2, #0; 
      BNE 		REPEAT5; 
      CMP 	R1, #0; 
      BEQ 		LEAVE3; 
      SUBS 	R1, #8; 
      CMP 	R1, #0; 
      BNE 	REPEAT4; 
LEAVE3
      POP {R0-R3}
	  BX    LR
	  
	  
      ALIGN      										; make sure the end of this section is aligned
      END        										; end of file

