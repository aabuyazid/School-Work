/*
    Name 1: Your Name
    UTEID 1: Your UTEID
*/

/***************************************************************/
/*                                                             */
/*   LC-3b Simulator                                           */
/*                                                             */
/*   EE 460N - Lab 5                                           */
/*   The University of Texas at Austin                         */
/*                                                             */
/***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************/
/*                                                             */
/* Files:  ucode        Microprogram file                      */
/*         pagetable    page table in LC-3b machine language   */
/*         isaprogram   LC-3b machine language program file    */
/*                                                             */
/***************************************************************/

/***************************************************************/
/* These are the functions you'll have to write.               */
/***************************************************************/

void eval_micro_sequencer();
void cycle_memory();
void eval_bus_drivers();
void drive_bus();
void latch_datapath_values();

/***************************************************************/
/* A couple of useful definitions.                             */
/***************************************************************/
#define FALSE 0
#define TRUE  1

/***************************************************************/
/* Use this to avoid overflowing 16 bits on the bus.           */
/***************************************************************/
#define Low16bits(x) ((x) & 0xFFFF)

/***************************************************************/
/* Definition of the control store layout.                     */
/***************************************************************/
#define CONTROL_STORE_ROWS 64
#define INITIAL_STATE_NUMBER 18

/***************************************************************/
/* Definition of bit order in control store word.              */
/***************************************************************/
enum CS_BITS {
    IRD,
    COND1, COND0,
    J5, J4, J3, J2, J1, J0,
    LD_MAR,
    LD_MDR,
    LD_IR,
    LD_BEN,
    LD_REG,
    LD_CC,
    LD_PC,
    GATE_PC,
    GATE_MDR,
    GATE_ALU,
    GATE_MARMUX,
    GATE_SHF,
    PCMUX1, PCMUX0,
    DRMUX,
    SR1MUX,
    ADDR1MUX,
    ADDR2MUX1, ADDR2MUX0,
    MARMUX,
    ALUK1, ALUK0,
    MIO_EN,
    R_W,
    DATA_SIZE,
    LSHF1,
    COND2,
    LD_SSP,
    LD_USP,
    LD_EXC,
    LD_PSR,
    NOT_PSR15,
    LD_SEC,
    GateIN_EX,
    GateEXC,
    GatePSR,
    INC_OP1, INC_OP0,
    INPMUX1, INPMUX0,
    R6MUX,
    INC_DEC,
    EXCMUX,
    SET_EXC,
    LD_VA,
    LD_RW,
    LD_MDR1,
    LD_PROT,
    LD_VirEND,
    SET_MDR0,
    RW,
    VAMUX1, VAMUX0,
    VPPMMUX,
    VirEND3, VirEND2, VirEND1, VirEND0,
    VirDone,

/* MODIFY: you have to add all your new control signals */
            CONTROL_STORE_BITS
} CS_BITS;

/***************************************************************/
/* Functions to get at the control bits.                       */
/***************************************************************/
int GetIRD(int *x)           { return(x[IRD]); }
int GetCOND(int *x)          { return((x[COND1] << 1) + x[COND0]); }
int GetJ(int *x)             { return((x[J5] << 5) + (x[J4] << 4) +
                                      (x[J3] << 3) + (x[J2] << 2) +
                                      (x[J1] << 1) + x[J0]); }
int GetLD_MAR(int *x)        { return(x[LD_MAR]); }
int GetLD_MDR(int *x)        { return(x[LD_MDR]); }
int GetLD_IR(int *x)         { return(x[LD_IR]); }
int GetLD_BEN(int *x)        { return(x[LD_BEN]); }
int GetLD_REG(int *x)        { return(x[LD_REG]); }
int GetLD_CC(int *x)         { return(x[LD_CC]); }
int GetLD_PC(int *x)         { return(x[LD_PC]); }
int GetGATE_PC(int *x)       { return(x[GATE_PC]); }
int GetGATE_MDR(int *x)      { return(x[GATE_MDR]); }
int GetGATE_ALU(int *x)      { return(x[GATE_ALU]); }
int GetGATE_MARMUX(int *x)   { return(x[GATE_MARMUX]); }
int GetGATE_SHF(int *x)      { return(x[GATE_SHF]); }
int GetPCMUX(int *x)         { return((x[PCMUX1] << 1) + x[PCMUX0]); }
int GetDRMUX(int *x)         { return(x[DRMUX]); }
int GetSR1MUX(int *x)        { return(x[SR1MUX]); }
int GetADDR1MUX(int *x)      { return(x[ADDR1MUX]); }
int GetADDR2MUX(int *x)      { return((x[ADDR2MUX1] << 1) + x[ADDR2MUX0]); }
int GetMARMUX(int *x)        { return(x[MARMUX]); }
int GetALUK(int *x)          { return((x[ALUK1] << 1) + x[ALUK0]); }
int GetMIO_EN(int *x)        { return(x[MIO_EN]); }
int GetR_W(int *x)           { return(x[R_W]); }
int GetDATA_SIZE(int *x)     { return(x[DATA_SIZE]); }
int GetLSHF1(int *x)         { return(x[LSHF1]); }

int GetCOND2(int *x)         { return(x[COND2]); }
int GetLD_SSP(int *x)        { return(x[LD_SSP]); }
int GetLD_USP(int *x)        { return(x[LD_USP]); }
int GetLD_EXC(int *x)        { return(x[LD_EXC]); }
int GetLD_PSR(int *x)        { return(x[LD_PSR]); }
int GetNOT_PSR15(int *x)     { return(x[NOT_PSR15]); }
int GetLD_SEC(int *x)        { return(x[LD_SEC]) ;}
int GetGateIN_EX(int *x)     { return(x[GateIN_EX]); }
int GetGateEXC(int *x)       { return(x[GateEXC]) ;}
int GetGatePSR(int *x)       { return(x[GatePSR]); }
int GetINC_OP(int *x)        { return((x[INC_OP1] << 1) + x[INC_OP0]); }
int GetINPMUX(int *x)        { return((x[INPMUX1] << 1) + x[INPMUX0]); }
int GetR6MUX(int *x)         { return(x[R6MUX]); }
int GetINC_DEC(int *x)       { return(x[INC_DEC]); }
int GetEXCMUX(int *x)        { return(x[EXCMUX]); }
int GetSET_EXC(int *x)       { return(x[SET_EXC]); }

int GetLD_VA(int *x)         { return(x[LD_VA]);}
int GetLD_RW(int *x)         { return(x[LD_RW]);}
int GetLD_MDR1(int *x)       { return(x[LD_MDR1]);}
int GetLD_PROT(int *x)          { return(x[LD_PROT]);}
int GetLD_VirEND(int *x)     { return(x[LD_VirEND]);}
int GetSET_MDR0(int *x)      { return(x[SET_MDR0]);}
int GetRW(int *x)            { return(x[RW]);}
int GetVAMUX(int *x)         { return((x[VAMUX1] << 1) + x[VAMUX0]);}
int GetVMPMMUX(int *x)       { return(x[VPPMMUX]);}
int GetVirEND(int *x)        { return((x[VirEND3] << 3) + (x[VirEND2] << 2) + (x[VirEND1] << 1) + x[VirEND0]);}
int GetVirDone(int *x)       { return(x[VirDone]);}
/* MODIFY: you can add more Get functions for your new control signals */

/***************************************************************/
/* The control store rom.                                      */
/***************************************************************/
int CONTROL_STORE[CONTROL_STORE_ROWS][CONTROL_STORE_BITS];

/***************************************************************/
/* Main memory.                                                */
/***************************************************************/
/* MEMORY[A][0] stores the least significant byte of word at word address A
   MEMORY[A][1] stores the most significant byte of word at word address A
   There are two write enable signals, one for each byte. WE0 is used for
   the least significant byte of a word. WE1 is used for the most significant
   byte of a word. */

#define WORDS_IN_MEM    0x2000 /* 32 frames */
#define MEM_CYCLES      5
int MEMORY[WORDS_IN_MEM][2];

/***************************************************************/

/***************************************************************/

/***************************************************************/
/* LC-3b State info.                                           */
/***************************************************************/
#define LC_3b_REGS 8

int RUN_BIT;	/* run bit */
int BUS;	/* value of the bus */

typedef struct System_Latches_Struct{

    int PC,		/* program counter */
            MDR,	/* memory data register */
            MAR,	/* memory address register */
            IR,		/* instruction register */
            N,		/* n condition bit */
            Z,		/* z condition bit */
            P,		/* p condition bit */
            BEN;        /* ben register */

    int READY;	/* ready bit */
    /* The ready bit is also latched as you dont want the memory system to assert it
       at a bad point in the cycle*/

    int REGS[LC_3b_REGS]; /* register file. */

    int MICROINSTRUCTION[CONTROL_STORE_BITS]; /* The microintruction */

    int STATE_NUMBER; /* Current State Number - Provided for debugging */

/* For lab 4 */
    int INTV; /* Interrupt vector register */
    int EXCV; /* Exception vector register */
    int SSP; /* Initial value of system stack pointer */

    int USP; /* Intial value of user stack pointer */
    int PSR; /* Initial value of process status register */

    int SLT_EXC;
    int REG_EXC;

    int INTF;
    int EXCF;
/* MODIFY: you should add here any other registers you need to implement interrupts and exceptions */

/* For lab 5 */
    int PTBR; /* This is initialized when we load the page table */
    int VA;   /* Temporary VA register */
    int RW;
    int PROT;
    int VIR_END;
/* MODIFY: you should add here any other registers you need to implement virtual memory */

} System_Latches;

/* Data Structure for Latch */

System_Latches CURRENT_LATCHES, NEXT_LATCHES;

/* For lab 5 */
#define PAGE_NUM_BITS 9
#define PTE_PFN_MASK 0x3E00
#define PTE_VALID_MASK 0x0004
#define PAGE_OFFSET_MASK 0x1FF

/***************************************************************/
/* A cycle counter.                                            */
/***************************************************************/
int CYCLE_COUNT;

/***************************************************************/
/*                                                             */
/* Procedure : help                                            */
/*                                                             */
/* Purpose   : Print out a list of commands.                   */
/*                                                             */
/***************************************************************/
void help() {
    printf("----------------LC-3bSIM Help-------------------------\n");
    printf("go               -  run program to completion       \n");
    printf("run n            -  execute program for n cycles    \n");
    printf("mdump low high   -  dump memory from low to high    \n");
    printf("rdump            -  dump the register & bus values  \n");
    printf("?                -  display this help menu          \n");
    printf("quit             -  exit the program                \n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : cycle                                           */
/*                                                             */
/* Purpose   : Execute a cycle                                 */
/*                                                             */
/***************************************************************/
void cycle() {

    eval_micro_sequencer();
    cycle_memory();
    eval_bus_drivers();
    drive_bus();
    latch_datapath_values();

    CURRENT_LATCHES = NEXT_LATCHES;

    CYCLE_COUNT++;
}

/***************************************************************/
/*                                                             */
/* Procedure : run n                                           */
/*                                                             */
/* Purpose   : Simulate the LC-3b for n cycles.                 */
/*                                                             */
/***************************************************************/
void run(int num_cycles) {
    int i;

    if (RUN_BIT == FALSE) {
        printf("Can't simulate, Simulator is halted\n\n");
        return;
    }

    printf("Simulating for %d cycles...\n\n", num_cycles);
    for (i = 0; i < num_cycles; i++) {
        if (CURRENT_LATCHES.PC == 0x0000) {
            RUN_BIT = FALSE;
            printf("Simulator halted\n\n");
            break;
        }
        cycle();
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : go                                              */
/*                                                             */
/* Purpose   : Simulate the LC-3b until HALTed.                 */
/*                                                             */
/***************************************************************/
void go() {
    if (RUN_BIT == FALSE) {
        printf("Can't simulate, Simulator is halted\n\n");
        return;
    }

    printf("Simulating...\n\n");
    while (CURRENT_LATCHES.PC != 0x0000)
        cycle();
    RUN_BIT = FALSE;
    printf("Simulator halted\n\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : mdump                                           */
/*                                                             */
/* Purpose   : Dump a word-aligned region of memory to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void mdump(FILE * dumpsim_file, int start, int stop) {
    int address; /* this is a byte address */

    printf("\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
    printf("-------------------------------------\n");
    for (address = (start >> 1); address <= (stop >> 1); address++)
        printf("  0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
    printf("\n");

    /* dump the memory contents into the dumpsim file */
    fprintf(dumpsim_file, "\nMemory content [0x%0.4x..0x%0.4x] :\n", start, stop);
    fprintf(dumpsim_file, "-------------------------------------\n");
    for (address = (start >> 1); address <= (stop >> 1); address++)
        fprintf(dumpsim_file, " 0x%0.4x (%d) : 0x%0.2x%0.2x\n", address << 1, address << 1, MEMORY[address][1], MEMORY[address][0]);
    fprintf(dumpsim_file, "\n");
    fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : rdump                                           */
/*                                                             */
/* Purpose   : Dump current register and bus values to the     */
/*             output file.                                    */
/*                                                             */
/***************************************************************/
void rdump(FILE * dumpsim_file) {
    int k;

    printf("\nCurrent register/bus values :\n");
    printf("-------------------------------------\n");
    printf("Cycle Count  : %d\n", CYCLE_COUNT);
    printf("PC           : 0x%0.4x\n", CURRENT_LATCHES.PC);
    printf("IR           : 0x%0.4x\n", CURRENT_LATCHES.IR);
    printf("STATE_NUMBER : 0x%0.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
    printf("BUS          : 0x%0.4x\n", BUS);
    printf("MDR          : 0x%0.4x\n", CURRENT_LATCHES.MDR);
    printf("MAR          : 0x%0.4x\n", CURRENT_LATCHES.MAR);
    printf("CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
    printf("Registers:\n");
    for (k = 0; k < LC_3b_REGS; k++)
        printf("%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
    printf("\n");

    /* dump the state information into the dumpsim file */
    fprintf(dumpsim_file, "\nCurrent register/bus values :\n");
    fprintf(dumpsim_file, "-------------------------------------\n");
    fprintf(dumpsim_file, "Cycle Count  : %d\n", CYCLE_COUNT);
    fprintf(dumpsim_file, "PC           : 0x%0.4x\n", CURRENT_LATCHES.PC);
    fprintf(dumpsim_file, "IR           : 0x%0.4x\n", CURRENT_LATCHES.IR);
    fprintf(dumpsim_file, "STATE_NUMBER : 0x%0.4x\n\n", CURRENT_LATCHES.STATE_NUMBER);
    fprintf(dumpsim_file, "BUS          : 0x%0.4x\n", BUS);
    fprintf(dumpsim_file, "MDR          : 0x%0.4x\n", CURRENT_LATCHES.MDR);
    fprintf(dumpsim_file, "MAR          : 0x%0.4x\n", CURRENT_LATCHES.MAR);
    fprintf(dumpsim_file, "CCs: N = %d  Z = %d  P = %d\n", CURRENT_LATCHES.N, CURRENT_LATCHES.Z, CURRENT_LATCHES.P);
    fprintf(dumpsim_file, "Registers:\n");
    for (k = 0; k < LC_3b_REGS; k++)
        fprintf(dumpsim_file, "%d: 0x%0.4x\n", k, CURRENT_LATCHES.REGS[k]);
    fprintf(dumpsim_file, "\n");
    fflush(dumpsim_file);
}

/***************************************************************/
/*                                                             */
/* Procedure : get_command                                     */
/*                                                             */
/* Purpose   : Read a command from standard input.             */
/*                                                             */
/***************************************************************/
void get_command(FILE * dumpsim_file) {
    char buffer[20];
    int start, stop, cycles;

    printf("LC-3b-SIM> ");

    scanf("%s", buffer);
    printf("\n");

    switch(buffer[0]) {
        case 'G':
        case 'g':
            go();
            break;

        case 'M':
        case 'm':
            scanf("%i %i", &start, &stop);
            mdump(dumpsim_file, start, stop);
            break;

        case '?':
            help();
            break;
        case 'Q':
        case 'q':
            printf("Bye.\n");
            exit(0);

        case 'R':
        case 'r':
            if (buffer[1] == 'd' || buffer[1] == 'D')
                rdump(dumpsim_file);
            else {
                scanf("%d", &cycles);
                run(cycles);
            }
            break;

        default:
            printf("Invalid Command\n");
            break;
    }
}

/***************************************************************/
/*                                                             */
/* Procedure : init_control_store                              */
/*                                                             */
/* Purpose   : Load microprogram into control store ROM        */
/*                                                             */
/***************************************************************/
void init_control_store(char *ucode_filename) {
    FILE *ucode;
    int i, j, index;
    char line[200];

    printf("Loading Control Store from file: %s\n", ucode_filename);

    /* Open the micro-code file. */
    if ((ucode = fopen(ucode_filename, "r")) == NULL) {
        printf("Error: Can't open micro-code file %s\n", ucode_filename);
        exit(-1);
    }

    /* Read a line for each row in the control store. */
    for(i = 0; i < CONTROL_STORE_ROWS; i++) {
        if (fscanf(ucode, "%[^\n]\n", line) == EOF) {
            printf("Error: Too few lines (%d) in micro-code file: %s\n",
                   i, ucode_filename);
            exit(-1);
        }

        /* Put in bits one at a time. */
        index = 0;

        for (j = 0; j < CONTROL_STORE_BITS; j++) {
            /* Needs to find enough bits in line. */
            if (line[index] == '\0') {
                printf("Error: Too few control bits in micro-code file: %s\nLine: %d\n",
                       ucode_filename, i);
                exit(-1);
            }
            if (line[index] != '0' && line[index] != '1') {
                printf("Error: Unknown value in micro-code file: %s\nLine: %d, Bit: %d\n",
                       ucode_filename, i, j);
                exit(-1);
            }

            /* Set the bit in the Control Store. */
            CONTROL_STORE[i][j] = (line[index] == '0') ? 0:1;
            index++;
        }

        /* Warn about extra bits in line. */
        if (line[index] != '\0')
            printf("Warning: Extra bit(s) in control store file %s. Line: %d\n",
                   ucode_filename, i);
    }
    printf("\n");
}

/***************************************************************/
/*                                                             */
/* Procedure : init_memory                                     */
/*                                                             */
/* Purpose   : Zero out the memory array                       */
/*                                                             */
/***************************************************************/
void init_memory() {
    int i;

    for (i=0; i < WORDS_IN_MEM; i++) {
        MEMORY[i][0] = 0;
        MEMORY[i][1] = 0;
    }
}

/**************************************************************/
/*                                                            */
/* Procedure : load_program                                   */
/*                                                            */
/* Purpose   : Load program and service routines into mem.    */
/*                                                            */
/**************************************************************/
void load_program(char *program_filename, int is_virtual_base) {
    FILE * prog;
    int ii, word, program_base, pte, virtual_pc;

    /* Open program file. */
    prog = fopen(program_filename, "r");
    if (prog == NULL) {
        printf("Error: Can't open program file %s\n", program_filename);
        exit(-1);
    }

    /* Read in the program. */
    if (fscanf(prog, "%x\n", &word) != EOF)
        program_base = word >> 1;
    else {
        printf("Error: Program file is empty\n");
        exit(-1);
    }

    if (is_virtual_base) {
        if (CURRENT_LATCHES.PTBR == 0) {
            printf("Error: Page table base not loaded %s\n", program_filename);
            exit(-1);
        }

        /* convert virtual_base to physical_base */
        virtual_pc = program_base << 1;
        pte = (MEMORY[(CURRENT_LATCHES.PTBR + (((program_base << 1) >> PAGE_NUM_BITS) << 1)) >> 1][1] << 8) |
              MEMORY[(CURRENT_LATCHES.PTBR + (((program_base << 1) >> PAGE_NUM_BITS) << 1)) >> 1][0];

        printf("virtual base of program: %04x\npte: %04x\n", program_base << 1, pte);
        if ((pte & PTE_VALID_MASK) == PTE_VALID_MASK) {
            program_base = (pte & PTE_PFN_MASK) | ((program_base << 1) & PAGE_OFFSET_MASK);
            printf("physical base of program: %x\n\n", program_base);
            program_base = program_base >> 1;
        } else {
            printf("attempting to load a program into an invalid (non-resident) page\n\n");
            exit(-1);
        }
    }
    else {
        /* is page table */
        CURRENT_LATCHES.PTBR = program_base << 1;
    }

    ii = 0;
    while (fscanf(prog, "%x\n", &word) != EOF) {
        /* Make sure it fits. */
        if (program_base + ii >= WORDS_IN_MEM) {
            printf("Error: Program file %s is too long to fit in memory. %x\n",
                   program_filename, ii);
            exit(-1);
        }

        /* Write the word to memory array. */
        MEMORY[program_base + ii][0] = word & 0x00FF;
        MEMORY[program_base + ii][1] = (word >> 8) & 0x00FF;;
        ii++;
    }

    if (CURRENT_LATCHES.PC == 0 && is_virtual_base)
        CURRENT_LATCHES.PC = virtual_pc;

    printf("Read %d words from program into memory.\n\n", ii);
}

/***************************************************************/
/*                                                             */
/* Procedure : initialize                                      */
/*                                                             */
/* Purpose   : Load microprogram and machine language program  */
/*             and set up initial state of the machine         */
/*                                                             */
/***************************************************************/
void initialize(char *argv[], int num_prog_files) {
    int i;
    init_control_store(argv[1]);

    init_memory();
    load_program(argv[2],0);
    for ( i = 0; i < num_prog_files; i++ ) {
        load_program(argv[i + 3],1);
    }
    CURRENT_LATCHES.Z = 1;
    CURRENT_LATCHES.STATE_NUMBER = INITIAL_STATE_NUMBER;
    memcpy(CURRENT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[INITIAL_STATE_NUMBER], sizeof(int)*CONTROL_STORE_BITS);
    CURRENT_LATCHES.SSP = 0x3000; /* Initial value of system stack pointer */
    CURRENT_LATCHES.PSR = 0x8002;

/* MODIFY: you can add more initialization code HERE */

    NEXT_LATCHES = CURRENT_LATCHES;

    RUN_BIT = TRUE;
}

/***************************************************************/
/*                                                             */
/* Procedure : main                                            */
/*                                                             */
/***************************************************************/
int main(int argc, char *argv[]) {
    FILE * dumpsim_file;

    /* Error Checking */
    if (argc < 4) {
        printf("Error: usage: %s <micro_code_file> <page table file> <program_file_1> <program_file_2> ...\n",
               argv[0]);
        exit(1);
    }

    printf("LC-3b Simulator\n\n");

    initialize(argv, argc - 3);

    if ( (dumpsim_file = fopen( "dumpsim", "w" )) == NULL ) {
        printf("Error: Can't open dumpsim file\n");
        exit(-1);
    }

    while (1)
        get_command(dumpsim_file);

}

/***************************************************************/
/* Do not modify the above code, except for the places indicated
   with a "MODIFY:" comment.
   You are allowed to use the following global variables in your
   code. These are defined above.

   CONTROL_STORE
   MEMORY
   BUS

   CURRENT_LATCHES
   NEXT_LATCHES

   You may define your own local/global variables and functions.
   You may use the functions to get at the control bits defined
   above.

   Begin your code here 	  			       */
/***************************************************************/

int SEXT(int x) {
    int ret = x;
    if(x & 0x8000) {
        ret += 0xFFFF0000;
    }
    return ret;
}

void eval_micro_sequencer() {

    /*
     * Evaluate the address of the next state according to the
     * micro sequencer logic. Latch the next microinstruction.
     */
    int J = (GetJ(CURRENT_LATCHES.MICROINSTRUCTION));
    int COND = GetCOND(CURRENT_LATCHES.MICROINSTRUCTION);
    int COND0 = COND & 0b1;
    int COND1 = (COND & 0b10) >> 1;

    if(GetIRD(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.STATE_NUMBER = (CURRENT_LATCHES.IR & 0xF000) >> 12;
    else NEXT_LATCHES.STATE_NUMBER = J | (((COND1 && ~COND0 && CURRENT_LATCHES.BEN)<<2) +
                                          ((~COND1 && COND0 && CURRENT_LATCHES.READY)<<1) + (COND1 && COND0 && ((CURRENT_LATCHES.IR)&0x0800)));

    NEXT_LATCHES.STATE_NUMBER = (GetVirDone(CURRENT_LATCHES.MICROINSTRUCTION)) ? CURRENT_LATCHES.VIR_END : NEXT_LATCHES.STATE_NUMBER;

    int INTEXC_Codes = ((GetCOND2(CURRENT_LATCHES.MICROINSTRUCTION) ? (CURRENT_LATCHES.INTF << 1) : 0)) + CURRENT_LATCHES.EXCF;
    switch(INTEXC_Codes) {
        case 0b10:
            NEXT_LATCHES.STATE_NUMBER = 45;
            CURRENT_LATCHES.INTF = 0;
            break;
        case 0b01:
            NEXT_LATCHES.STATE_NUMBER = 36;
            CURRENT_LATCHES.INTF = 0;
            break;
        case 0b11:
            NEXT_LATCHES.STATE_NUMBER = 36;
            CURRENT_LATCHES.INTF = 0;
            break;
        default:
            NEXT_LATCHES.STATE_NUMBER = NEXT_LATCHES.STATE_NUMBER;
    }
    memcpy(NEXT_LATCHES.MICROINSTRUCTION, CONTROL_STORE[NEXT_LATCHES.STATE_NUMBER], sizeof(NEXT_LATCHES.MICROINSTRUCTION));


}

int input_MDR;
void cycle_memory() {

    /*
     * This function emulates memory and the WE logic.
     * Keep track of which cycle of MEMEN we are dealing with.
     * If fourth, we need to latch Ready bit at the end of
     * cycle to prepare microsequencer for the fifth cycle.
     */

    static unsigned int counter = 0;
    int mem_control = (GetLD_MDR(CURRENT_LATCHES.MICROINSTRUCTION) << 3) +
                      (GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION) << 2) +
                      (GetR_W(CURRENT_LATCHES.MICROINSTRUCTION) << 1) +
                      GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION);
    if(mem_control&0b0100) {
        if (counter == MEM_CYCLES - 1) {
            switch (mem_control) {
                case 0b1101:    // Read from mem and put word into MDR
                    input_MDR = (Low16bits(MEMORY[CURRENT_LATCHES.MAR>>1][0] + (MEMORY[CURRENT_LATCHES.MAR>>1][1]<<8)));
                    NEXT_LATCHES.MDR = input_MDR;
                    break;
                case 0b0111:    // Write from MDR to M[MAR]
                    MEMORY[CURRENT_LATCHES.MAR>>1][1] = SEXT(Low16bits(CURRENT_LATCHES.MDR & 0xFF00) >> 8);
                    MEMORY[CURRENT_LATCHES.MAR>>1][0] = SEXT(Low16bits(CURRENT_LATCHES.MDR & 0x00FF));
                    break;
                case 0b0110:    // Write Byte from MDR to M[MAR]
                    if(CURRENT_LATCHES.MAR & 0x0001) {
                        int MDR_byte = Low16bits((CURRENT_LATCHES.MDR & 0xFF00)) >> 8;
                        MEMORY[CURRENT_LATCHES.MAR >> 1][1] = MDR_byte;
                    }
                    else {
                        int MDR_byte = Low16bits(CURRENT_LATCHES.MDR & 0x00FF);
                        MEMORY[CURRENT_LATCHES.MAR >> 1][0] = MDR_byte;
                    }
                    break;
                default:
                    break;
            }
            NEXT_LATCHES.READY = 0;
        }
        if (counter == MEM_CYCLES - 2) NEXT_LATCHES.READY = 1;
        counter = (counter+1) % (MEM_CYCLES);
    }
}


int input_ALU, input_SHF, input_MARMUX, input_EXC, input_IN_EX, input_EXC_LOGIC;

void eval_bus_drivers() {

    /*
     * Datapath routine emulating operations before driving the bus.
     * Evaluate the input of tristate drivers
     *             Gate_MARMUX,
     *		 Gate_PC,
     *		 Gate_ALU,
     *		 Gate_SHF,
     *		 Gate_MDR.
     */

    int SR1, OP2;

    SR1 = (CURRENT_LATCHES.IR & 0x0E00) >> 9;
    if(GetSR1MUX(CURRENT_LATCHES.MICROINSTRUCTION)) SR1 = (CURRENT_LATCHES.IR & 0x01C0) >> 6;
    if(GetR6MUX(CURRENT_LATCHES.MICROINSTRUCTION)) SR1 = 6;

    if(CURRENT_LATCHES.IR & 0x0020) {
        OP2 = CURRENT_LATCHES.IR & 0x001F;
        if(OP2 & 0x0010) OP2 += 0xFFFFFFE0;
    }
    else {
        OP2 = CURRENT_LATCHES.IR & 0x0007;
        OP2 = CURRENT_LATCHES.REGS[OP2];
        if(OP2 & 0x8000) OP2 += 0xFFFF0000;
    }

    switch (GetALUK(CURRENT_LATCHES.MICROINSTRUCTION)) {
        case 0b00:
            input_ALU = CURRENT_LATCHES.REGS[SR1] + OP2;
            break;
        case 0b01:
            input_ALU = CURRENT_LATCHES.REGS[SR1] & OP2;
            break;
        case 0b10:
            input_ALU = CURRENT_LATCHES.REGS[SR1] ^ OP2;
            break;
        default:
            input_ALU = CURRENT_LATCHES.REGS[SR1];
    }

    input_ALU = Low16bits(input_ALU);

    int SHR_Codes = (CURRENT_LATCHES.IR & 0x0030) >> 4;
    int amount4 = CURRENT_LATCHES.IR & 0x000F;

    switch (SHR_Codes) {
        case 0b00:
            input_SHF = CURRENT_LATCHES.REGS[SR1] << amount4;
            break;
        case 0b01:
            input_SHF = CURRENT_LATCHES.REGS[SR1] >> amount4;
            break;
        case 0b11:
            input_SHF = CURRENT_LATCHES.REGS[SR1];
            if(CURRENT_LATCHES.REGS[SR1] & 0x8000) {
                for (int i = 0; i < amount4; i++) {
                    input_SHF >>= 1;
                    input_SHF += 0x80000000;
                }
            }
            else input_SHF = CURRENT_LATCHES.REGS[SR1] >> amount4;
            break;

    }

    input_SHF = Low16bits(input_SHF);

    int ADDR1_val = 0;
    if(GetADDR1MUX(CURRENT_LATCHES.MICROINSTRUCTION)) ADDR1_val = CURRENT_LATCHES.REGS[SR1];
    else ADDR1_val = CURRENT_LATCHES.PC;

    int ADDR2_val = 0;
    switch (GetADDR2MUX(CURRENT_LATCHES.MICROINSTRUCTION)) {
        case 1:
            ADDR2_val = CURRENT_LATCHES.IR & 0x003F;
            if(ADDR2_val & 0x0020) ADDR2_val += 0xFFFFFFC0;
            break;
        case 2:
            ADDR2_val = CURRENT_LATCHES.IR & 0x01FF;
            if(ADDR2_val & 0x0100) ADDR2_val += 0xFFFFFE00;
            break;
        case 3:
            ADDR2_val = CURRENT_LATCHES.IR & 0x07FF;
            if(ADDR2_val & 0x0400) ADDR2_val += 0xFFFFF800;
            break;
        default:
            ADDR2_val = 0;
    }
    if(GetLSHF1(CURRENT_LATCHES.MICROINSTRUCTION)) ADDR2_val <<= 1;

    int ADDR_result = Low16bits(ADDR1_val + ADDR2_val);

    int ZEXT = (CURRENT_LATCHES.IR & 0x00FF) << 1;

    input_MARMUX = (GetMARMUX(CURRENT_LATCHES.MICROINSTRUCTION)) ? ADDR_result:ZEXT;

    if(GetLD_PC(CURRENT_LATCHES.MICROINSTRUCTION)) {
        switch (GetPCMUX(CURRENT_LATCHES.MICROINSTRUCTION)) {
            case 1:
                NEXT_LATCHES.PC = BUS;
                break;
            case 2:
                NEXT_LATCHES.PC = ADDR_result;
                break;
            case 3:
                NEXT_LATCHES.PC = CURRENT_LATCHES.PC - 2;
                break;
            default:
                NEXT_LATCHES.PC = CURRENT_LATCHES.PC + 2;
                break;
        }
    }
    else NEXT_LATCHES.PC = CURRENT_LATCHES.PC;

    if(GetGateEXC(CURRENT_LATCHES.MICROINSTRUCTION))
        if(GetVMPMMUX(CURRENT_LATCHES.MICROINSTRUCTION)) input_EXC = CURRENT_LATCHES.MDR;
        else input_EXC = CURRENT_LATCHES.MAR;
    else input_EXC = 0x10000;

    int input_EXCMUX = 0;

    if(GetVMPMMUX(CURRENT_LATCHES.MICROINSTRUCTION)) {
        if(CURRENT_LATCHES.PROT && !((input_EXC & 0x0004) >> 2)) input_EXCMUX = 3;
        else if(!(input_EXC & 0x0004)) input_EXCMUX = 1;
    }
    else if(input_EXC & 0x0001 && GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION)) input_EXCMUX = 2;

    input_EXC_LOGIC = (GetEXCMUX(CURRENT_LATCHES.MICROINSTRUCTION)) ? 4 : input_EXCMUX;

    int input_EXC_ADR;
    switch(CURRENT_LATCHES.SLT_EXC) {
        case 1:
            input_EXC_ADR = 0x02;
            break;
        case 2:
            input_EXC_ADR = 0x03;
            break;
        case 3:
            input_EXC_ADR = 0x04;
            break;
        case 4:
            input_EXC_ADR = 0x05;
            break;
        default: input_EXC_ADR = 0;
    }

    input_IN_EX = 0x200 + ((((!CURRENT_LATCHES.REG_EXC) ? 0x01:input_EXC_ADR)) << 1);

}


void drive_bus() {

    /*
     * Datapath routine for driving the bus from one of the 5 possible
     * tristate drivers.
     */

    int Bus_Codes = ((GetGateIN_EX(CURRENT_LATCHES.MICROINSTRUCTION) << 6) +
                    (GetGatePSR(CURRENT_LATCHES.MICROINSTRUCTION) << 5) +
                    (GetGATE_PC(CURRENT_LATCHES.MICROINSTRUCTION) << 4) +
                    (GetGATE_MDR(CURRENT_LATCHES.MICROINSTRUCTION) << 3) +
                    (GetGATE_ALU(CURRENT_LATCHES.MICROINSTRUCTION) << 2) +
                    (GetGATE_MARMUX(CURRENT_LATCHES.MICROINSTRUCTION) << 1) +
                    GetGATE_SHF(CURRENT_LATCHES.MICROINSTRUCTION));
    switch (Bus_Codes) {
        case 0b1000000:
            BUS = Low16bits(input_IN_EX);
            break;
        case 0b0100000:
            BUS = CURRENT_LATCHES.PSR;
            break;
        case 0b0010000:
            BUS = CURRENT_LATCHES.PC;
            break;
        case 0b0001000:
            if(!GetDATA_SIZE(CURRENT_LATCHES.MICROINSTRUCTION)) {
                if(CURRENT_LATCHES.MAR & 0x0001) BUS = Low16bits((CURRENT_LATCHES.MDR & 0xFF00) >> 8);
                else BUS = Low16bits((CURRENT_LATCHES.MDR & 0x00FF));
                if(BUS & 0x0080) BUS += 0xFF00;
            }
            else BUS = CURRENT_LATCHES.MDR;
            break;
        case 0b0000100:
            BUS = Low16bits(input_ALU);
            break;
        case 0b0000010:
            BUS = Low16bits(input_MARMUX);
            break;
        case 0b0000001:
            BUS = Low16bits(input_SHF);
            break;
        default:
            BUS = 0;
    }
    if (GetGateEXC(CURRENT_LATCHES.MICROINSTRUCTION)) input_EXC = BUS;
}

void latch_datapath_values() {

    /*
     * Datapath routine for computing all functions that need to latch
     * values in the data path at the end of this cycle.  Some values
     * require sourcing the bus; therefore, this routine has to come
     * after drive_bus.
     */

    int MAR_bit = (CURRENT_LATCHES.MAR & 0x1);
    int CC, DR;
    if(GetLD_MAR(CURRENT_LATCHES.MICROINSTRUCTION)) {
        switch (GetVAMUX(CURRENT_LATCHES.MICROINSTRUCTION)) {
            case 1:
                NEXT_LATCHES.MAR = (CURRENT_LATCHES.PTBR & 0xFF00) + ((CURRENT_LATCHES.VA & 0xFE00) >> 8);
                break;
            case 3:
                NEXT_LATCHES.MAR = (CURRENT_LATCHES.MDR & 0x3E00) + (CURRENT_LATCHES.VA & 0x01FF);
                break;
            default:
                NEXT_LATCHES.MAR = BUS;
        }
    }
    if(GetLD_MDR(CURRENT_LATCHES.MICROINSTRUCTION)) {
        if (!GetMIO_EN(CURRENT_LATCHES.MICROINSTRUCTION)) {
            switch (MAR_bit) {
                case 0:
                    NEXT_LATCHES.MDR = BUS;
                    break;
                default:
                    NEXT_LATCHES.MDR = (((BUS & 0x00FF) << 8) + ((BUS & 0xFF00) >> 8));
                    NEXT_LATCHES.MDR = Low16bits(NEXT_LATCHES.MDR);
                    break;
            }
        }
    }

    if(GetLD_MDR1(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.MDR = (CURRENT_LATCHES.MDR & 0xFFFD) + (CURRENT_LATCHES.RW << 1);

    if(GetSET_MDR0(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.MDR = (CURRENT_LATCHES.MDR & 0xFFFE) + 1;

    if (GetLD_IR(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.IR = BUS;

    if (GetLD_BEN(CURRENT_LATCHES.MICROINSTRUCTION)) {
        CC = (CURRENT_LATCHES.N << 2) + (CURRENT_LATCHES.Z << 1) + CURRENT_LATCHES.P;
        NEXT_LATCHES.BEN = ((CURRENT_LATCHES.IR & 0x0E00) >> 9 & CC) != 0;
    }
    if (GetLD_REG(CURRENT_LATCHES.MICROINSTRUCTION)) {
        if (GetDRMUX(CURRENT_LATCHES.MICROINSTRUCTION)) DR = 7;
        else DR = (CURRENT_LATCHES.IR & 0x0E00) >> 9;
        if (GetR6MUX(CURRENT_LATCHES.MICROINSTRUCTION)) DR = 6;
        switch (GetINPMUX(CURRENT_LATCHES.MICROINSTRUCTION)) {
            case 0b01:
                NEXT_LATCHES.REGS[DR] = Low16bits(CURRENT_LATCHES.SSP);
                break;
            case 0b10:
                NEXT_LATCHES.REGS[DR] = Low16bits(CURRENT_LATCHES.USP);
                break;
            case 0b11:
                NEXT_LATCHES.REGS[DR] = Low16bits((GetINC_DEC(CURRENT_LATCHES.MICROINSTRUCTION)) ? CURRENT_LATCHES.REGS[6]+2 : CURRENT_LATCHES.REGS[6]-2);
                break;
            default:
            NEXT_LATCHES.REGS[DR] = BUS;
        }
    }
    if (GetLD_CC(CURRENT_LATCHES.MICROINSTRUCTION)) {
        if (SEXT(BUS) < 0) {
            NEXT_LATCHES.N = 1;
            NEXT_LATCHES.Z = 0;
            NEXT_LATCHES.P = 0;
        } else if (SEXT(BUS) == 0) {
            NEXT_LATCHES.N = 0;
            NEXT_LATCHES.Z = 1;
            NEXT_LATCHES.P = 0;
        } else {
            NEXT_LATCHES.N = 0;
            NEXT_LATCHES.Z = 0;
            NEXT_LATCHES.P = 1;
        }
        NEXT_LATCHES.PSR = (CURRENT_LATCHES.PSR & 0xFFF8) + (NEXT_LATCHES.N << 2) + (NEXT_LATCHES.Z << 1) + NEXT_LATCHES.P;
    }
    if(GetLD_PC(CURRENT_LATCHES.MICROINSTRUCTION)) {
        if (GetPCMUX(CURRENT_LATCHES.MICROINSTRUCTION) == 1) NEXT_LATCHES.PC = BUS;
    }

    if(GetLD_PSR(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.PSR = BUS;

    if(GetLD_SSP(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.SSP = CURRENT_LATCHES.REGS[6];

    if(GetLD_USP(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.USP = CURRENT_LATCHES.REGS[6];


    if(GetLD_VirEND(CURRENT_LATCHES.MICROINSTRUCTION)) {
        switch (GetVirEND(CURRENT_LATCHES.MICROINSTRUCTION)) {
            case 0b0000:
                NEXT_LATCHES.VIR_END = 33;
                break;
            case 0b1111:
                NEXT_LATCHES.VIR_END = 28;
                break;
            case 0b0100:
                NEXT_LATCHES.VIR_END = 29;
                break;
            case 0b0101:
                NEXT_LATCHES.VIR_END = 25;
                break;
            case 0b0010:
                NEXT_LATCHES.VIR_END = 24;
                break;
            case 0b0011:
                NEXT_LATCHES.VIR_END = 23;
                break;
            case 0b1000:
                NEXT_LATCHES.VIR_END = 61;
                break;
            case 0b1001:
                NEXT_LATCHES.VIR_END = 48;
                break;
            case 0b1100:
                NEXT_LATCHES.VIR_END = 57;
                break;
            case 0b1101:
                NEXT_LATCHES.VIR_END = 40;
                break;
            case 0b1110:
                NEXT_LATCHES.VIR_END = 44;
                break;
        }

    }

    if(GetLD_VA(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.VA = CURRENT_LATCHES.MAR;

    if(GetLD_RW(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.RW = GetRW(CURRENT_LATCHES.MICROINSTRUCTION);

    if(GetLD_PROT(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.PROT = (BUS & 0x8000) >> 15;


    if(GetLD_EXC(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.REG_EXC = (GetSET_EXC(CURRENT_LATCHES.MICROINSTRUCTION)) ? 1:0;

    if(GetLD_SEC(CURRENT_LATCHES.MICROINSTRUCTION)) {
        NEXT_LATCHES.SLT_EXC = input_EXC_LOGIC;
    }

    if(GetNOT_PSR15(CURRENT_LATCHES.MICROINSTRUCTION)) NEXT_LATCHES.PSR = CURRENT_LATCHES.PSR & 0x7FFF;

    if((input_EXC_LOGIC & 0x0001) || ((input_EXC_LOGIC & 0x0002) >> 1) || input_EXC_LOGIC >> 2) NEXT_LATCHES.STATE_NUMBER = 36;

    if(CYCLE_COUNT == 300) NEXT_LATCHES.INTF = 1;
    else NEXT_LATCHES.INTF = CURRENT_LATCHES.INTF;

}
