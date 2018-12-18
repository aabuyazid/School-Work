        .ORIG x1200
        
        LEA R5, COUNT
        LDW R4, R5, #0
        LEA R5, PTBR
        LDW R1, R5, #0
        LEA R5, CLEAR
        LDW R3, R5, #0
        
LOOP    LDW R0, R1, #0
        AND R0, R0, R3
        STW R0, R1, #0
        ADD R0, R0, #2
        ADD R4, R4, #-1
        BRz LOOP
        
        RTI

COUNT   .FILL x0080
PTBR    .FILL x1000
CLEAR   .FILL xFFFE

	.END