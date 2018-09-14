; SysTick.s
; Module written by: Arkan Abuyazid & Michael Hernandez
; Date Created: 2/14/2017
; Last Modified: 3/01/2017 
; Brief Description: Initializes SysTick

NVIC_ST_CTRL_R        EQU 0xE000E010
NVIC_ST_RELOAD_R      EQU 0xE000E014
NVIC_ST_CURRENT_R     EQU 0xE000E018
TICKS				  EQU 0x00FFFFFF

        AREA    |.text|, CODE, READONLY, ALIGN=2
        THUMB
; ;-UUU-Export routine(s) from SysTick.s to callers
		EXPORT SysTick_Init
;------------SysTick_Init------------
; ;-UUU-Complete this subroutine
; Initialize SysTick with busy wait running at bus clock.
; Input: none
; Output: none
; Modifies: ??

SysTick_Init
															; Set Enable bit to 0
	LDR 		R0,=NVIC_ST_CTRL_R;
	LDR 		R1, [R0];
	AND 		R1, R1, #0xFE;
	STR 		R1, [R0];
															; 10 million ticks is .125 ms in 80MHz
	LDR 		R0,=NVIC_ST_RELOAD_R;
	LDR 		R1, =TICKS
	STR 		R1, [R0];
	
															; Reset Current Register
	LDR 		R0,=NVIC_ST_CURRENT_R;
	STR 		R1,[R0]										; Attempting to store a number in CURRENT_R
															; Resets it automatically
															
															; Turn on counter
	LDR 		R0,=NVIC_ST_CTRL_R
	LDR 		R1, [R0]
	ORR			R1, R1, #0x00000005						; Enable bit = 1, Clock bit = 1
	AND 		R1, R1, #0xFFFFFFFD						; Interrupt bit = 0;
	STR			R1, [R0]
	
    BX  LR                  					        ; return


    ALIGN                           					; make sure the end of this section is aligned
    END                             						; end of file
