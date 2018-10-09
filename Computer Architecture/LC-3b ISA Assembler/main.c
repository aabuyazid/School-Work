/*
	Name 1:  Arkan Abuyazid
	Name 2 : Michael Hernandez
	UTEID 1: ata757
	UTEID 2: moh363
*/

#include <stdio.h> /* standard input/output library */
#include <stdint.h>
#include <stdlib.h> /* Standard C Library */
#include <string.h> /* String operations library */
#include <ctype.h> /* Library for useful character operations */
#include <limits.h> /* Library for definitions of common variable type characteristics */

#define MAX_LINE_LENGTH 255
#define MAX_LABEL_LEN 20
#define MAX_SYMBOLS 255
#define NUM_OPS 37

FILE* infile = NULL;
FILE* outfile = NULL;
FILE * pOutfile = NULL;

enum
{
    DONE, OK, EMPTY_LINE
};

/* Creates Symbol table entry*/
typedef struct {
    int address;
    char label[MAX_LABEL_LEN + 1];	/* Question for the reader: Why do we need to add 1? */
} TableEntry;
TableEntry symbolTable[MAX_SYMBOLS];

const char ops[NUM_OPS][10] = {"add", "and", "br", "brn", "brz", "brp", "brnz",
                   "brnp", "brzp", "brnzp", "jmp", "jsr", "jsrr", "ldb",
                   "ldw", "lea", "rti", "ret", "lshf", "rshfl", "rshfa",
                   "stb", "stw", "trap", "xor", "halt", "nop", "not",
                   ".fill", ".blkw", ".orig", ".stringz", ".end", "getc", "puts",
                   "in", "out"
};

/* generates symbol table */
void FirstPass(void);
/* Adds an element to the symbol table and checks if it is already there*/
int makeSymbolTable(char* pLabel, int memLocation, int currentStore);

/* Checks if label is anything it should not be*/
void CheckValidLabel(char* pLabel);

/* generates 1's and 0's */
void SecondPass(void);

int readAndParse(FILE* pInfile, char* pLine, char** pLabel,  char** pOpcode, char** pArg1, char** pArg2, char** pArg3, char** pArg4);

int isOpcode(char* input);

int whichOpCode(char* opcode);

/* looks at if input is .end*/
int isEnd(char* input);

/* Looks at if input is .orig*/
int isFront(char* input);

/* checks the value depending on operation, .Orig is 29 and .fill is 30*/
int isOrig(char* input, int operation);

void WriteInstructions(void);

int main(int argc, char* argv[]) {

    char *prgName   = NULL;
    char *iFileName = NULL;
    char *oFileName = NULL;
    int lInstr = 0;
    prgName   = argv[0];
    iFileName = argv[1];
    oFileName = argv[2];

    int inFileLen = strlen(iFileName);
    int outFileLen = strlen(oFileName);


    printf("program name = '%s'\n", prgName);
    printf("input file name = '%s'\n", iFileName);
    printf("output file name = '%s'\n", oFileName);


    /* open the source file */
    infile = fopen(argv[1], "r");
    outfile = fopen(argv[2], "w");

    if (!infile) {
        printf("Error: Cannot open file %s\n", argv[1]);
        exit(4);
    }
    if (!outfile) {
        printf("Error: Cannot open file %s\n", argv[2]);
        exit(4);
    }

    /* Do stuff with files */
    FirstPass();
    rewind(infile);
    SecondPass();
    rewind(infile);
    pOutfile = fopen( oFileName, "w" );
    WriteInstructions();


    fclose(infile);
    fclose(outfile);



    return 0;
}

int toNum( char * pStr )
{
    char * t_ptr;
    char * orig_pStr;
    int t_length,k;
    int lNum, lNeg = 0;
    long int lNumLong;

    orig_pStr = pStr;
    if( *pStr == '#' )				/* decimal */
    {
        pStr++;
        if( *pStr == '-' )				/* dec is negative */
        {
            lNeg = 1;
            pStr++;
        }
        t_ptr = pStr;
        t_length = strlen(t_ptr);
        for(k=0;k < t_length;k++)
        {
            if (!isdigit(*t_ptr))
            {
                printf("Error: invalid decimal operand, %s\n",orig_pStr);
                exit(4);
            }
            t_ptr++;
        }
        lNum = atoi(pStr);
        if (lNeg)
            lNum = -lNum;

        return lNum;
    }
    else if( *pStr == 'x' )	/* hex     */
    {
        pStr++;
        if( *pStr == '-' )				/* hex is negative */
        {
            lNeg = 1;
            pStr++;
        }
        t_ptr = pStr;
        t_length = strlen(t_ptr);
        for(k=0;k < t_length;k++)
        {
            if (!isxdigit(*t_ptr))
            {
                printf("Error: invalid hex operand, %s\n",orig_pStr);
                exit(4);
            }
            t_ptr++;
        }
        lNumLong = strtol(pStr, NULL, 16);    /* convert hex string into integer */
        lNum = (lNumLong > INT_MAX)? INT_MAX : lNumLong;
        if( lNeg )
            lNum = -lNum;
        return lNum;
    }
    else
    {
        printf( "Error: invalid operand, %s\n", orig_pStr);
        exit(4);  /* This has been changed from error code 3 to error code 4, see clarification 12 */
    }
}

int readAndParse( FILE * pInfile, char * pLine, char ** pLabel, char** pOpcode, char ** pArg1, char ** pArg2, char ** pArg3, char ** pArg4){
    char * lRet, * lPtr;
    int i;
    if( !fgets( pLine, MAX_LINE_LENGTH, pInfile ) )
        return( DONE );
    for( i = 0; i < strlen( pLine ); i++ )
        pLine[i] = tolower( pLine[i] );

    /* convert entire line to lowercase */
    *pLabel = *pOpcode = *pArg1 = *pArg2 = *pArg3 = *pArg4 = pLine + strlen(pLine);

    /* ignore the comments */
    lPtr = pLine;

    while( *lPtr != ';' && *lPtr != '\0' &&
           *lPtr != '\n' )
        lPtr++;

    *lPtr = '\0';
    if( !(lPtr = strtok( pLine, "\t\n ," ) ) )
        return( EMPTY_LINE );

    if( isOpcode( lPtr ) == -1 && lPtr[0] != '.' ) /* found a label */
    {
        *pLabel = lPtr;
        if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );
    }

    *pOpcode = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg1 = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg2 = lPtr;
    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg3 = lPtr;

    if( !( lPtr = strtok( NULL, "\t\n ," ) ) ) return( OK );

    *pArg4 = lPtr;

    return( OK );
}

/* Note: MAX_LINE_LENGTH, OK, EMPTY_LINE, and DONE are defined values */

void FirstPass(void) {
    char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
            *lArg2, *lArg3, *lArg4;

    int lRet;

    //FILE * lInfile;

    //lInfile = fopen( "data.in", "r" );	/* open the input file */
/* checks the first line to see if it is .orig */
    lRet = readAndParse(infile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4);
    if(*lLabel != '\0'){
        exit(4);
    }
    int front = isFront(lOpcode);
    if(front == -1)
        exit (4);
    int programCounter = isOrig(lArg1, 29);
   // if(invalidOrig == -1)
     //   exit (3);
    if(*lArg2 != '\0')
        exit(4);

    int symbolLabelIndex = 0;
    do
    {
        lRet = readAndParse( infile, lLine, &lLabel,
                             &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
        if( lRet != DONE && lRet != EMPTY_LINE )
        {
            if(*lLabel != '\0' ) {
                CheckValidLabel(lLabel);
                symbolLabelIndex = makeSymbolTable(lLabel, programCounter, symbolLabelIndex);
            }
            //Check for case when .orig is xFFFF or close to that
            programCounter += 2;
            if(isEnd(lOpcode)){
                lRet = DONE;
                continue;
            }
        }
    } while( lRet != DONE );
}

// For ADD, AND, XOR, and NOT
void CheckInstruction_Group1(int numOpcode, char *pArg1, char *pArg2, char *pArg3, char *pArg4) {
    long DR = 0; long SR1 = 0; long SR2 = 0; char imm5 = 0; char mask = 0x10;

    if(*pArg1 != 'r' || *pArg2 != 'r') exit(4);

    DR = strtol((pArg1+1),NULL,10);
    SR1 = strtol((pArg2+1),NULL,10);
    if(DR < 0 || DR > 7 || SR1 < 0 || SR1 > 7) exit(4);

    if(*pArg4 != '\0') exit(4);

    if(numOpcode != 28) {
        switch (*pArg3) {
            case 'r' :
                SR2 = strtol((pArg3 + 1), NULL, 10);
                break;
            case '#' :
                imm5 = toNum(pArg3);
                break;
            case 'x' :
                imm5 = toNum(pArg3);
                if(*(pArg3+1) != '-' && (mask & imm5)) imm5 += 0xE0;
                break;
            default:
                exit(4);
        }
        if (SR2 < 0 || SR2 > 7) exit(4);
        if (imm5 < -16 || imm5 > 15) exit(3);
    }

}

// For LDB LDW STB STW
void CheckInstruction_Group2(int numOpcode, char *pArg1, char *pArg2, char *pArg3, char *pArg4) {
    long DR = 0; long BaseR = 0; char offset6 = 0; char mask = 0x20;

    if(*pArg1 != 'r' || *pArg2 != 'r') exit(4);

    DR = strtol((pArg1+1),NULL,10);
    BaseR = strtol((pArg2+1),NULL,10);
    if(DR < 0 || DR > 7 || BaseR < 0 || BaseR > 7) exit(4);

    if(*pArg4 != '\0') exit(4);

    switch (*pArg3) {
        case '#' :
            offset6 =toNum(pArg3);
            break;
        case 'x' :
            offset6 =toNum(pArg3);
            if(*(pArg3+1) != '-' && (mask & offset6))offset6 += 0xC0;
            break;
        default :
            exit(4);
    }
    if (offset6 < -32 || offset6 > 31) exit(3);
}

// For JMP JSRR
void CheckInstruction_Group3(char *pArg1, char *pArg2, char *pArg3, char *pArg4) {
    long BaseR = 0;

    if(*pArg1 != 'r') exit(4);

    if(*pArg2 != '\0' || *pArg3 != '\0' || *pArg4 != '\0') exit(4);

    BaseR = strtol((pArg1+1),NULL,10);
    if(BaseR < 0 || BaseR > 7) exit(4);
}

// For JSR TRAP
void CheckInstruction_Group4(int numOpcode, char *pArg1, char *pArg2, char *pArg3, char *pArg4, int programCounter) {
    int PCoffset11 = 0; int trapvect8 = 0; int foundLabel = 0;

    if(*pArg2 != '\0' || *pArg3 != '\0' || *pArg4 != '\0') exit(4);
    //For JSR
    if(numOpcode == 12) {
        if(*pArg1 == 'x') exit(4);
        for (int index = 0; index < MAX_SYMBOLS; index++) {
            if (strcmp(symbolTable[index].label, pArg1) == 0) {
                PCoffset11 = symbolTable[index].address - programCounter;
                foundLabel = 1;
                break;
            }
        }
        PCoffset11 /= 2;
        if (PCoffset11 < -1024 || PCoffset11 > 1023) exit(4);
        if(!foundLabel) exit(1);
    }
    else {
        if(*pArg1 != 'x' && *pArg1 != '#') exit(4);
        trapvect8 = toNum(pArg1);
        if(trapvect8 < 0 || trapvect8 > 255) exit(3);
    }
}

// For RTI RET
void CheckInstruction_Group5(char *pArg1) {
    if(*pArg1 != '\0') exit(4);
}
// For LSHF RSHFL RSHFA
void CheckInstruction_Group6(char *pArg1, char *pArg2, char *pArg3, char *pArg4) {
    long DR = 0; long SR = 0; long amount4 = 0;

    if(*pArg1 != 'r' || *pArg2 != 'r') exit(4);

    DR = strtol((pArg1+1),NULL,10);
    SR = strtol((pArg2+1),NULL,10);
    if(DR < 0 || DR > 7 || SR < 0 || SR > 7) exit(4);

    if(*pArg4 != '\0') exit(4);

    switch (*pArg3) {
        case '#' :
            amount4 = toNum(pArg3);
            break;
        case 'x' :
            amount4 = toNum(pArg3);
            break;
        default :
            exit(4);
    }

    if(amount4 < 0 || amount4 > 15) exit(3);
}

// For ALL BR and LEA
void CheckInstruction_Group7(char *pArg1, char *pArg2, char *pArg3, char *pArg4, int programCounter) {
    int PCoffset9 = 0; char* pLabel = NULL; int DR = 0; int foundLabel = 0; int isLEA = 0;
    if(isalnum(*pArg2)) {
        if(*pArg1 != 'r') exit(4);
        DR = strtol((pArg1+1),NULL,10);
        if(DR < 0 || DR > 7) exit(4);
        pLabel = pArg2;
        isLEA = 1;
        if(*pArg3 != '\0' || *pArg4 != '\0') exit(4);
    }
    else {
        if(*pArg2 != '\0' || *pArg3 != '\0' || *pArg4 != '\0') exit(4);
        pLabel = pArg1;
    }

    for(int index = 0; index < MAX_SYMBOLS; index++) {
        if(strcmp(symbolTable[index].label, pLabel) == 0) {
            PCoffset9 = symbolTable[index].address - programCounter;
            foundLabel = 1;
            break;
        }
    }

    if(!foundLabel) exit(1);
    if(isLEA && (PCoffset9 < -256 || PCoffset9 > 255)) exit(4);
    if(PCoffset9 < -1023 || PCoffset9 > 1023) exit(4);
}

// For .FILL .BLKW
void CheckPseudoOp(int numOpcode, char *pArg1, char* pArg2) {
    int32_t word = 0;
    if(*pArg2 != '\0') exit(4);
    if(numOpcode == 29) {
        if (*pArg1 != 'x' && *pArg1 != '#') exit(4);
        word = toNum(pArg1);
        if(word < -32768 || word > 32767) exit(3);
    }
}

// For ADD, AND, XOR, and NOT
uint16_t CreateInstruction_Group1(int numOpcode, char *pArg1, char *pArg2, char *pArg3) {
    char DR = 0; char SR1 = 0; char SR2 = 0; char imm5 = 0; uint16_t instruction = 0; char mask = 0x10; int imm5_en = 0;
    switch (numOpcode) {
        case 1:
            instruction = 0x01;
            break;
        case 2:
            instruction = 0x05;
            break;
        case 25:
            instruction = 0x09;
            break;
        case 28:
            instruction = 0x09;
            break;
    }
    instruction <<= 3;

    DR = strtol((pArg1+1),NULL,10);
    SR1 = strtol((pArg2+1),NULL,10);

    instruction += (DR & 0x07);
    instruction <<= 3;
    instruction += (SR1 & 0x07);

    switch (*pArg3) {
        case 'r' :
            SR2 = strtol((pArg3+1),NULL,10);
            break;
        case '#' :
            imm5 = toNum(pArg3);
            imm5_en = 1;
            break;
        case 'x' :
            imm5 = toNum(pArg3);
            if(*(pArg3+1) != '-' && (mask & imm5)) imm5 += 0xE0;
            imm5_en = 1;
            break;
    }

    if(numOpcode == 28) {
        imm5 = 0b11111;
        imm5_en = 1;
    }

    if(imm5_en) {
        instruction <<= 1;
        instruction += 1;
        instruction <<= 5;
        instruction += (imm5 & 0x1F);
    }
    else {
        instruction <<= 6;
        instruction += SR2;
    }

    return instruction;
}

// For LDB LDW STB STW
uint16_t CreateInstruction_Group2(int numOpcode, char *pArg1, char *pArg2, char *pArg3) {
    long DR = 0; long BaseR = 0; char offset6 = 0; char mask = 0x20; uint16_t instruction = 0;

    switch (numOpcode) {
        case 14 :
            instruction = 2;
            break;
        case 15 :
            instruction = 6;
            break;
        case 22 :
            instruction = 3;
            break;
        case 23:
            instruction = 7;
            break;
    }

    instruction <<= 3;

    DR = strtol((pArg1+1),NULL,10);
    instruction += DR;
    instruction <<= 3;

    BaseR = strtol((pArg2+1),NULL,10);
    instruction += BaseR;
    instruction <<= 6;

    switch (*pArg3) {
        case '#' :
            offset6 =toNum(pArg3);
            break;
        case 'x' :
            offset6 =toNum(pArg3);
            if(*(pArg3+1) != '-' && (mask & offset6))offset6 += 0xC0;
            break;
        default :
            exit(4);
    }
    if(offset6 < 0) offset6 &= 0x3F;
    instruction += offset6;

    return instruction;

}

// For JMP JSRR
uint16_t CreateInstruction_Group3(int numOpcode, char *pArg1) {
    uint16_t instruction = 0;
    long BaseR = strtol((pArg1+1),NULL,10);
    instruction = (numOpcode == 11) ? 12 : 4;

    instruction <<= 6;

    instruction += BaseR;

    instruction <<= 6;

    return instruction;

}

// For JSR TRAP
uint16_t CreateInstruction_Group4(int numOpcode, char* pArg1, int programCounter) {
    int PCoffset11 = 0; int trapvect8 = 0; uint16_t instruction = 0;
    if(numOpcode == 12) {
        instruction = 4;
        instruction <<= 1;
        instruction += 1;
        instruction <<= 11;
        for (int index = 0; index < MAX_SYMBOLS; index++) {
            if (strcmp(symbolTable[index].label, pArg1) == 0) {
                PCoffset11 = symbolTable[index].address - programCounter;
                break;
            }
        }
        PCoffset11 /= 2;
        instruction += PCoffset11;
    }
    else {
        instruction = 0b1111;
        instruction <<= 12;
        trapvect8 = toNum(pArg1);
        instruction += trapvect8;
    }

    return instruction;

}

uint16_t CreateInstruction_Group5(int numOpcode) {
    switch (numOpcode) {
        case 17:
            return 0x8000;
        case 18:
            return 0xC1C0;
        case 27:
            return 0x0000;
    }
}

// For LSHF RSHFL RSHFA
uint16_t CreateInstruction_Group6(int numOpcode, char* pArg1, char* pArg2, char* pArg3) {
    uint16_t instruction = 13; long DR = 0; long SR = 0; long amount4 = 0;

    instruction <<= 3;

    DR = strtol((pArg1+1),NULL,10);
    SR = strtol((pArg2+1),NULL,10);

    instruction += DR;
    instruction <<= 3;
    instruction += SR;
    instruction <<= 2;

    switch(numOpcode) {
        case 20:
            instruction += 1;
            break;
        case 21:
            instruction += 3;
            break;
        default:
            break;
    }

    instruction <<= 4;

    switch (*pArg3) {
        case '#' :
            amount4 = toNum(pArg3);
            break;
        case 'x' :
            amount4 = toNum(pArg3);
            break;
        default :
            exit(4);
    }

    instruction += amount4;

    return instruction;
}

// For All BR and LEA
uint16_t CreateInstruction_Group7(int numOpcode, char* pArg1, char* pArg2, int programCounter) {
    int PCoffset9 = 0; char* pLabel = NULL; int DR = 0; int isLEA = 0; uint16_t instruction = 0;
    if(isalnum(*pArg2)) {
        DR = strtol((pArg1+1),NULL,10);
        pLabel = pArg2;
        isLEA = 1;
    }
    else {
        pLabel = pArg1;
    }
    for(int index = 0; index < MAX_SYMBOLS; index++) {
        if(strcmp(symbolTable[index].label, pLabel) == 0) {
            PCoffset9 = symbolTable[index].address - programCounter;
            break;
        }
    }

    PCoffset9 >>= 1;

    instruction = (isLEA) ? 0b1110 : 0;
    instruction <<= 3;
    if(isLEA) instruction += DR;
    else {
        switch (numOpcode) {
            case 3:
                instruction += 0b111;
                break;
            case 4:
                instruction += 0b100;
                break;
            case 5:
                instruction += 0b010;
                break;
            case 6:
                instruction += 0b001;
                break;
            case 7:
                instruction += 0b110;
                break;
            case 8:
                instruction += 0b101;
                break;
            case 9:
                instruction += 0b011;
                break;
            case 10:
                instruction += 0b111;
                break;
        }
    }
    instruction <<= 9;
    instruction += (PCoffset9 & 0x01FF);
    return instruction;
}

// For .FILL .BLKW (ONLY .FILL FOR NOW)
uint16_t CreatePsuedoOps(int numOpcode, char* pArg1) {
    uint16_t instruction = 0; int word = 0;
    if(numOpcode == 29) {
        word = toNum(pArg1);
        word &= 0x0000FFFF;
        instruction = word;
        return instruction;
    }
    return 0;
}

// For HALT GETC PUTS IN OUT

uint16_t CreateInstructions_TRAP(int numOpcode) {
    switch (numOpcode) {
        case 26:
            return 0xF025;
        case 34:
            return 0xF020;
        case 35:
            return 0xF022;
        case 36:
            return 0xF023;
        case 37:
            return 0xF021;
    }
}
void SecondPass(void) {
    char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
            *lArg2, *lArg3, *lArg4;

    int lRet= readAndParse( infile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );

    if(*lLabel != '\0'){
        exit(4);
    }
    int front = isFront(lOpcode);
    if(front == -1)
        exit (4);

    int programCounter = isOrig(lArg1, 29);
    if(*lArg2 != '\0')
        exit(4);
    int symbolTableIndex = 0;

    // Size of all instructions will be 7 chars long (including NULL character)

    do
    {
        lRet = readAndParse( infile, lLine, &lLabel,
                             &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
        if( lRet != DONE && lRet != EMPTY_LINE )
        {
            int numOpcode = isOpcode(lOpcode);

            // For ADD, AND, XOR, and NOT
            if(numOpcode == 1 || numOpcode == 2 || numOpcode == 25 || numOpcode == 28) {
                CheckInstruction_Group1(numOpcode, lArg1, lArg2, lArg3, lArg4);
            }
            // For LDB LDW STB STW
            else if(numOpcode == 14 || numOpcode == 15 || numOpcode == 22 || numOpcode == 23) {
                CheckInstruction_Group2(numOpcode, lArg1, lArg2, lArg3, lArg4);
            }
            // For JMP JSRR
            else if(numOpcode == 11 || numOpcode == 13) {
                CheckInstruction_Group3(lArg1, lArg2, lArg3, lArg4);
            }
            // For JSR TRAP
            else if(numOpcode == 12 || numOpcode == 24) {
                CheckInstruction_Group4(numOpcode, lArg1, lArg2, lArg3, lArg4, programCounter);
            }
            // For RTI RET NOP
            else if(numOpcode == 17 || numOpcode == 18 || numOpcode == 27) {
                CheckInstruction_Group5(lArg1);
            }
            // For LSHF RSHFL RSHFA
            else if(numOpcode == 19 || numOpcode == 20 || numOpcode == 21) {
                CheckInstruction_Group6(lArg1, lArg2, lArg3, lArg4);
            }
            // For All BR and LEA
            else if((numOpcode >= 3 && numOpcode <= 10) || numOpcode == 16) {
                CheckInstruction_Group7(lArg1, lArg2, lArg3, lArg4, programCounter);
            }
            // For .FILL .BLKW
            else if(numOpcode == 29 || numOpcode == 30) {
                CheckPseudoOp(numOpcode, lArg1, lArg2);
            }
            else if(isOpcode(lOpcode) == -1) {
                exit(2);
            }
            else if(isEnd(lOpcode)) {
                lRet = DONE;
                continue;
            }
            programCounter += 2;
        }
    } while( lRet != DONE );
}

void WriteInstructions(void) {
    char lLine[MAX_LINE_LENGTH + 1], *lLabel, *lOpcode, *lArg1,
            *lArg2, *lArg3, *lArg4;

    int lRet= readAndParse( infile, lLine, &lLabel, &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );

    if(*lLabel != '\0'){
        exit(4);
    }
    int front = isFront(lOpcode);
    if(front == -1)
        exit (4);

    int programCounter = isOrig(lArg1, 29);

    if(*lArg2 != '\0') exit(4);

    printf("0x%.4X\n", toNum(lArg1));

    fprintf( pOutfile, "0x%.4X\n", toNum(lArg1) );	/* where lInstr is declared as an int */

    int symbolTableIndex = 0;

    uint16_t instruction = 0;

    // Size of all instructions will be 7 chars long (including NULL character)

    do
    {
        lRet = readAndParse( infile, lLine, &lLabel,
                             &lOpcode, &lArg1, &lArg2, &lArg3, &lArg4 );
        if( lRet != DONE && lRet != EMPTY_LINE )
        {
            int numOpcode = isOpcode(lOpcode);

            if(numOpcode == 1 || numOpcode == 2 || numOpcode == 25 || numOpcode == 28) {
                instruction = CreateInstruction_Group1(numOpcode, lArg1, lArg2, lArg3);
            }
                // For LDB LDW STB STW
            else if(numOpcode == 14 || numOpcode == 15 || numOpcode == 22 || numOpcode == 23) {
                instruction = CreateInstruction_Group2(numOpcode, lArg1, lArg2, lArg3);
            }
                // For JMP JSRR
            else if(numOpcode == 11 || numOpcode == 13) {
                instruction = CreateInstruction_Group3(numOpcode, lArg1);
            }
                // For JSR TRAP
            else if(numOpcode == 12 || numOpcode == 24) {
                instruction = CreateInstruction_Group4(numOpcode, lArg1, programCounter+2);
            }
                // For RTI RET NOP
            else if(numOpcode == 17 || numOpcode == 18 || numOpcode == 27) {
                instruction = CreateInstruction_Group5(numOpcode);
            }
                // For LSHF RSHFL RSHFA
            else if(numOpcode == 19 || numOpcode == 20 || numOpcode == 21) {
                instruction = CreateInstruction_Group6(numOpcode, lArg1, lArg2, lArg3);
            }
                // For All BR and LEA
            else if((numOpcode >= 3 && numOpcode <= 10) || numOpcode == 16) {
                instruction = CreateInstruction_Group7(numOpcode, lArg1, lArg2, programCounter+2);
            }
                // For .FILL .BLKW
            else if(numOpcode == 29 || numOpcode == 30) {
                instruction = CreatePsuedoOps(numOpcode, lArg1);
            }

            else if (numOpcode == 26 || (numOpcode >= 34 && numOpcode <= 37)) {
                instruction = CreateInstructions_TRAP(numOpcode);
            }
            else if(isEnd(lOpcode)) {
                lRet = DONE;
                continue;
            }
            programCounter += 2;

            printf("0x%.4X\n", instruction);

            fprintf( pOutfile, "0x%.4X\n", instruction );	/* where lInstr is declared as an int */
        }
    } while( lRet != DONE );
}

/* returns count if opcode match and returns -1 if no opcode match
 * uses double char array to store opcodes and compares the strings until
 * both reach NULL
 * count says which opcode out of the 28 it is
 * */
int isOpcode(char* input){

    int result = 0;
/*
 *	char* and = "and";
	char* add = "add";
	char* br = "br";
	char* halt = "halt";
	char* jmp = "jmp";
	char* jsr = "jsr";
	char* jsrr = "jsrr";
	char* ldb = "ldb";
	char* ldw = "ldw";
	char* lea = "lea";
	char* nop = "nop";
	char* not  "not";
	char* not = "ret";
	char* lshf = "lshf";
	char* rshfl = "rshfl";
	char* rshfa = "rshfa";
	char* rti = "rti";
	char* stb = "stb";
	char* stw = "stw";
	char* trap = "trap";
	char* xor = "xor";
	char* brnp = "brnp";
	char* brn = "brn";
	char* brnz = "brnz";
	char* brnzp = "brnzp";
	char* brz = "brz";
	char* brzp = "brzp";
	char* brp = "brp";
	char* temp = input;
 * */
    int count = 0;
    while(count < NUM_OPS){
        result = strcmp(input, ops[count]);
            if(result == 0) return (count + 1);
        count++;
    }
    return -1;
}

/*
 * Checks  if the input is the end, 1 if it is, -1 if it is not
 * */
int isEnd(char* input){
    char* ending = ".end";
        int result = strcmp(input, ending);
        if(result == 0){
            return 1;
    }
return 0;
}

int isFront(char* input){
    char* ending = ".orig";
        int result = strcmp(input, ending);
        if(result == 0){
            return 1;
    }
return -1;
}

/*Checks for signed and unsigned maxes. If negative, it checks if bigger than 32768
 * for unsigned 65535
 * returns 1 if it is not bigger
 * */
int isOrig(char* input, int operation){
    if(*input == '\0') return -1;
    unsigned int maxVal = 65535;
    int lowerVal = 32768;
    int inputVal = 0;
    int result = toNum(input);
    if((operation == 1) || (operation == 2) || (operation == 25)){
        /* -16 to 15 range for 2^5  */
        if((result > 15) || (result < -16)){
            exit(3);
        }
        return 1;
    }
    if((operation == 19) || (operation == 20) || (operation == 21)){
     if((result < 0) || (result > 15)) exit(3);
     return 1;
    }
    if((operation == 29) || (operation == 30)){
        if((result < -32768) || (result > 65535)) exit(3);
        if(operation == 29) {
            if ((result % 2) != 0) exit(3);
    }
    //printf("%x", result);
    return result;
    }
    if((operation < 0) || (operation > 30)){
        exit(2);
    }
    /* invalid immediate on opcode*/
    return -1; //if -1, there is an error, but I don't know what it is
}

/*
 * Checks if more than 255 labels
 * checks the length of label if greater than 20
 * makes copy of label and stores it (must we malloc?)
 * checks if already in symbol table
 * updates current stores
 * */
int makeSymbolTable(char* pLabel, int memLocation, int currentStore){
    if(currentStore >= MAX_SYMBOLS)
        exit(4);
    int length = strlen(pLabel);
    if(length > MAX_LABEL_LEN){
        exit(4);
    }
    for(int i = 0; i < currentStore + 1; i++){

        if(strcmp(pLabel, symbolTable[i].label) == 0) exit(4);
        //appends the string below for 21
    }
    strcpy(symbolTable[currentStore].label, pLabel);
    symbolTable[currentStore].address = memLocation;
    return (currentStore + 1);
}

/* returns 1 if a valid label*/
void CheckValidLabel(char* pLabel){
    char notLabels [4][5] = {"in", "out", "getc", "puts"}; //can it not be halt or so either

    for(int i = 0; i < 5; i++){
        if(strcmp(pLabel, notLabels[i]) == 0) exit(4);
    }
    for(int i = 0; i < NUM_OPS; i++){
        if(strcmp(pLabel, ops[i]) == 0) exit(4);
    }
    char c = pLabel[0];
    if(c == 'x') exit(4);
    //assumes it has null on end
    for(int i = 0; i < strlen(pLabel); i++){
        c = pLabel[i];
        if(isalnum(c) == 0) exit(4);
    }
}

/* returns 1 if the label was in the symbol table
 * checks the argument for the second one on BR, LEA, JSR, JSRR
 * exits 1 if the label is not in the symbol table
 * */
int doesLabelExist(char* pLabel){
    for(int i = 0; i < 255; i++){
        TableEntry t1 = symbolTable[i];
        if(strcmp(pLabel, t1.label)){
            return 1;
        }
    }
    exit(1);
}