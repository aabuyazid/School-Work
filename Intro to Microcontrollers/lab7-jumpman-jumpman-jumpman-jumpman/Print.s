; Print.s
; Student names: Arkan Abuyazid and Michael Hernandez
; Last modification date: change this to the last modification date or look very silly
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
	PRESERVE8
    THUMB
BIG					 DCB "*.***",0	
OVER 				EQU 0
DECIMAL				EQU 6
I					EQU 7
N					EQU 8
INPUT				EQU 0
DECIMALCHARACTER	EQU 4
NULL 				EQU 8	

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutDec
	  PUSH{ R11, LR}
FP RN 11;
	  MOV R11, SP
	  MOV R1, #10;
NOT_DONE
	  SUB SP, #8;
	  STR  R0, [SP, #INPUT];
	  UDIV R2, R0, R1;
	  MUL  R3, R2, R1;
	  SUB R3, R0, R3; MOD
	  STR R3, [SP, #DECIMALCHARACTER];
	  UDIV R0, R0, R1;
	  CMP  R0, #0;
	  BEQ  WRITE;
	  B NOT_DONE;
WRITE 
	  LDR R1, [SP, #DECIMALCHARACTER]
	  BL 			CONVERT
	  MOV R0, R1;
	  BL 			ST7735_OutChar;
	  ADD SP, #8
	  MOV R10, SP;
	  CMP R11, R10;
	  BNE 			WRITE;
	  
FINISHED  
	  POP {R11, LR}
      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix
     PUSH {R5, LR}
	 MOV R1, #10000
	 CMP R0, R1
	 BCS STRING
	 MOV R2, #1000
	 B TEST1
STRING 
	 LDR R0, =BIG
	 BL 			ST7735_OutString;
	 B      		DONE1
TEST1
FP RN 11						; Frame Pointer now denotes R11
	 SUB	SP, #12				;
	 LDR 	FP, [SP]
	 STR 	R0, [SP, #N]
	 MOV 	R0, #42
	 MOV 	R1, #46
	 MOV	R2, #0
	 MOV 	R0, #0
	 LDR 	R1, [SP, #N]
	 MOV	R2, #9999
	 CMP 	R1, R2
	 BLS 	OTHER
	 B 		DONE
OTHER
	 MOV	R3, 	#1000 		;
	 UDIV 	R0, R1, R3 			;
	 ADD	R2, R0, #0
	 MUL    R5, R0, R3       	;
	 SUB    R5, R1, R5
	 STR	R5, [SP, #N]      	; 
	 ADD	R0, R0, #0x30
	 BL				ST7735_OutChar
	 MOV	R0, 	#46
	 BL				ST7735_OutChar
	 MOV	R3, 	#1000
	 LDR	R1, [SP, #N]      	;
	 MUL	R0, R2, R3
	 SUB	R0, R1, R0
	 ADD	R1, R0, #0
	 STR	R1, [SP, #N]      	;
	 MOV	R3, 	#100  		;
	 UDIV 	R0, R1, R3 ;R4
	 MUL    R5, R0, R3       	;
	 SUB    R5, R1, R5
	 STR	R5, [SP, #N]      	; 
	 ADD	R0, R0, #0x30
	 BL				ST7735_OutChar
	 MOV	R3, 	#100
	 LDR	R1, [SP, #N]      	;
	 MUL	R0, R2, R3
	 SUB	R0, R1, R0
	 ADD	R1, R0, #0
	 STR	R1, [SP, #N]      	;
	 MOV	R3, 	#10 		;
	 UDIV 	R0, R1, R3 			; 
	 MUL    R5, R0, R3       	;
	 SUB    R5, R1, R5
	 STR	R5, [SP, #N]     	;
	 ADD	R0, R0, #0x30
	 BL				ST7735_OutChar
	 MOV	R3, 	#10
	 LDR	R1, [SP, #N]     	;
	 MUL	R0, R2, R3
	 SUB	R0, R1, R0
	 ADD	R1, R0, #0
	 STR	R1, [SP, #N]      	;
	 ADD	R0, R0, #0x30
	 BL				ST7735_OutChar
DONE	
	 ADD	SP, 	#12; 
	 POP 	{LR, R5}  ;R
     BX   	LR
TEST
     BX   	LR
DONE1
     POP 	{R5, LR}
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

; -----------------------CONVERT----------------------
; Converts Decimal to ASCII
; R1 is changed 
; Inputs:  none
; Outputs: none
CONVERT
	 PUSH   { R4, LR};
	 ADD 	R1, R1, #0x30;
	 POP 	{R4, LR};
	 BX 	LR;
     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
