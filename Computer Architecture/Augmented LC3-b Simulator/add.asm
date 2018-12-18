	    .ORIG x3000
        
	    LEA R5, SUM; 66
	    LDW R5, R5, #0;	 72
	    LEA R4, ARRAY; 9 cycles 97
	    LDW R4, R4, #0; 144
	    ADD R3, R0, #10; 169 cycles
	
LOOP    LDB R2, R4, #0; 241
        ADD R0, R0, R2; 266
	    LDB R2, R4, #1; 313
	    ADD R0, R0, R2; 338 x3010

	    ADD R4, R4, #2; 172
	    ADD R3, R3, #-1; 181

	    BRp LOOP; 389, 250, 310, 370, 430, 490, 550, 610, 670, 731
	
	    STW R0, R5, r#0; 740

        JMP R5

ARRAY	.FILL xC000; x301E
SUM	    .FILL xC014; x3020

	.END