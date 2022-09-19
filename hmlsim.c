//================================================
//
// JHMejia
//
// Hatchling Simulator
// 
//
//================================================

// HELLO WORLD


//================================================
// includes and defines
//================================================
#include <stdio.h>
#include <stdlib.h> // im not sure i ever needed this
#include <string.h>

//================================================
// global type statements
//================================================
struct hml {

    short int accumulator;
    unsigned short int instructReg;
    unsigned char instructCntr;
    unsigned char opCode;
    unsigned char operand;
    signed short mem[256];
    int insCount;
};
typedef struct hml Hatchling;
//================================================
// function prototypes
//================================================

Hatchling loadData(int argc, char *argv[], Hatchling empty)
{
    if (argc == 2) {
        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        size_t read;
        fp = fopen(argv[1], "r");
        if (fp == NULL)
            exit(EXIT_FAILURE);

            
        if (line)
            free(line);

        for (int i = 0 ; i < 255; i++)
        {
            empty.mem [i] = (unsigned short) 0;
        }

        


        empty.accumulator = (short) 0; 
        printf("Reading file....\n");
        while ((read = getline(&line, &len, fp)) != -1) {

        
            printf("%s", line);
            char dest[8];
            strncpy(dest, line, 7);
            dest[7] = '\0';
            //printf("%s", dest);

            char instrCount[3];
            char opCode[3];
            char operand[3];

            strncpy(instrCount , line, 2);
            instrCount[2] = '\0';
            strncpy(opCode, &dest[3], 2);
            strncpy(operand, &dest[5], 2);
            
            
            opCode[2] = '\0';

            
            operand[2] = '\0';

            int instrCountInt = (int) strtol(instrCount, NULL, 16);
            
            char opCodeAndOperand[5];
            strncpy(opCodeAndOperand, &dest[3], 4);
            opCodeAndOperand[4] = '\0';

            short AllShort= (short) strtol(opCodeAndOperand, NULL, 16);

            
            empty.mem[instrCountInt] = AllShort;
            

            }

        fclose(fp);
    } else {

        FILE *fp;
        int isWriting =1;
        fp = fopen("file.hml", "w+");
        char str[100];
        char end[9] = "-99999";
        while (isWriting)
        {
            fgets(str, 100, stdin);
            

            if (strstr(str, end) != NULL) {
                isWriting = 0;
            } else {
                fprintf(fp, "%s", str);
            }
            
            
        }
        fclose(fp);
        
        argv[1] = "file.hml";
        return loadData(2, argv, empty);
    }
    
       
    return empty;
    

};

//Helper function: will update a hatchling to reflect the current instruction better
Hatchling UpdateHatchling(Hatchling notUpdated) 
{
    unsigned char instr = notUpdated.instructCntr;
    int instrCountInt = (int) instr;

    short ops = notUpdated.mem[instrCountInt];
    notUpdated.instructReg = ops;
    char opcode;
    char operand;

    operand = (char) ops;
    ops = ops >> 8;
    opcode = ops;
    notUpdated.opCode = opcode;
    notUpdated.operand = operand;

    
    return notUpdated;

};


/** 
 * runData(Hatchling Loaded)
 * This will take a fully loaded hatchling and execute the program starting from memory location 0. 
 * 
 */

Hatchling runData(Hatchling loaded)
{
    //To do, get op code stuff 
    int isRunning = 1;
    while(isRunning) {
        loaded = UpdateHatchling(loaded);

        char hexOpCode[3];
        sprintf(hexOpCode, "%02X", loaded.opCode);
        char hex2[3];
        sprintf(hex2, "%02X", loaded.operand);

        //printf("The op code is %s and the operand is %s\n", hexOpCode, hex2);  TURN ON FOR DEBUG 
        unsigned short counter = (unsigned short) loaded.instructCntr;

        /**
         * Switch statement: gets op code and decides what to do based on it.
         * Op codes are unsigned char, so I have to turn the hex codes into chars.
         */
        switch (loaded.opCode) {
            case((unsigned char) 16): //ADD 0x10

                loaded.accumulator  = (unsigned short) loaded.accumulator + (unsigned short) loaded.mem[loaded.operand];
                counter ++;
                break;
            case((unsigned char) 17): //SUB 0x11

                loaded.accumulator  = (signed short) loaded.accumulator - (unsigned short) loaded.mem[loaded.operand];
                counter ++;
                break;
            case((unsigned char) 18): //MULT 0x12

                loaded.accumulator  = (signed short) loaded.accumulator * (unsigned short) loaded.mem[loaded.operand];
                counter ++;
                break;
            case((unsigned char) 19): //DIV 0x13

                if (((short) loaded.mem[loaded.operand]) == (short) 0){
                    printf("***FATAL ERROR - ATTEMPTING TO DIVIDE BY 0***\n");
                    isRunning = 0;
                    break;
                }
                loaded.accumulator  = (signed short) ((signed short) loaded.accumulator / (unsigned short) loaded.mem[loaded.operand]);
                counter ++;
                break;     
            case((unsigned char) 20): //MOD 0x14
                if (((short) loaded.mem[loaded.operand]) == (short) 0){
                    printf("***FATAL ERROR - ATTEMPTING TO MOD BY 0***\n");
                    isRunning = 0;
                    break;
                }
                loaded.accumulator  = (signed short) loaded.accumulator % (unsigned short) loaded.mem[loaded.operand];
                counter ++;
                break; 
            case((unsigned char) 32): //AND 0x20
                loaded.accumulator = (signed short)(loaded.accumulator & loaded.mem[loaded.operand]);
                counter++;
                break;
            case((unsigned char) 33): //ORR 0x21
                loaded.accumulator = (signed short) (loaded.accumulator | loaded.mem[loaded.operand]);
                counter++;
                break;
            case((unsigned char) 34): //NOT 0x22
                loaded.accumulator = (signed short) (!loaded.accumulator);
                counter++;
                break;
            case((unsigned char) 35): //XOR 0x23
                loaded.accumulator = (signed short) (loaded.accumulator ^ loaded.mem[loaded.operand]);
                counter++;
                break;
            case((unsigned char) 36): //LSR 0x23
                loaded.accumulator = (signed short) (((unsigned short) loaded.accumulator) >> 1);
                counter++;
                break;
            case((unsigned char) 37): //ASR 0x23
                loaded.accumulator = (signed short) (((signed short) loaded.accumulator) >> 1);
                counter++;
                break;
            case((unsigned char) 38): //LSL 0x23
                loaded.accumulator = (signed short) (loaded.accumulator << 1);
                counter++;
                break;                
            case ((unsigned char) 48): //BRANCH 0x30
                counter = (unsigned short) loaded.operand;
                break;
            case ((unsigned char) 49): //BRANCH NEG 0x31
                if (loaded.accumulator < 0 ) {
                    counter = (short) loaded.operand;
                } else {
                    counter++;
                }   
                break; 
            case ((unsigned char) 50): //BRANCH POS 0x31
                if (loaded.accumulator > 0 ) {
                    counter = (short) loaded.operand;
                } else {
                    counter++;
                }   
                break;
            case ((unsigned char) 51): //BRANCH ZERO 0x31
                if (loaded.accumulator == 0 ) {
                    counter = (short) loaded.operand;
                } else {
                    counter++;
                }   
                break;             
            case ('@'): //This is LOAD 0x40

                //printf("LOAD DATA\n");
                
                loaded.accumulator = loaded.mem[(short) loaded.operand];
                counter ++;
                

                break;
            case ('A'): //this is STOR 0x41
                //printf("STORING DATA\n");

                loaded.mem[(short) loaded.operand] = (unsigned short int) loaded.accumulator;
                counter ++;
                break;
            case ((unsigned char) 80): //READ (0x50)

                

                int readValue;
                printf("Input an int: ");
               
                scanf("%d", &readValue);

                

                //printf("The value you are tryint to store in mem is %d \n", readValue);
                

                loaded.mem[(short) loaded.operand] = (short) readValue; 
                counter++;
                break;


            case ((char) 81): //WRTE (0x51)
                char hexOpCode[5];
                sprintf(hexOpCode, "%04hX", loaded.mem[(short) loaded.operand]);
                printf("WRITE: %s\n", hexOpCode);
                counter ++;
                break;
            case ((unsigned char) 255): //HALT (0xFF)
                isRunning = 0;
                printf("*** PROGRAM EXECUTION TERMINATED  ***\n");
                break;
            default:
                printf("*** BAD INSTRUCTION ON LINE %02X ***\n", loaded.instructCntr);
                isRunning = 0;
                counter ++;

        }

        
        
        



        if (counter == 255) {
            isRunning = 0;
            printf("*** FATAL ERROR: REACHED END OF MEMORY WITHOUT FINDING HALT INTRUCTION ***\n");
        }
        loaded.instructCntr = (char) counter;

    };
    return loaded;
};


/**
 * This function takes a finished hatchling and prints out all relevant data: 
 * for example it will print out the op code, operand, etc. and all current memory. Easy!
 * 
 */
void printData(Hatchling finished) 
{


    printf("\nREGISTERS:\n");
    char    hexOpCode[3];
    char    hexOperand[3];
    char    hexInstructReg[5];
    char    hexAcc[5];
    char    hexInstructCounter[3];
    sprintf(hexOpCode, "%02hX", finished.opCode);
    sprintf(hexOperand, "%02hX", finished.operand);
    sprintf(hexInstructReg, "%04hX", finished.instructReg);
    sprintf(hexAcc, "%04hX", finished.accumulator);
    sprintf(hexInstructCounter, "%02hX", (unsigned short) finished.instructCntr + 1);

    printf("ACC     %s\n", hexAcc);
    printf("InstCtr   %s\n", hexInstructCounter);
    printf("InstReg %s\n", hexInstructReg);
    printf("OpCode    %s\n", hexOpCode);
    printf("Operand   %s\n\nMEMORY: \n\n       ", hexOperand);

    
    for (int k = 0; k < 16; k++)
    {
        printf("%2X   ", k);
    }
    printf("\n");
    for (int i = 0; i < 16; i ++)
    {
        printf("%2X0  ", i);
        for (int j = 0; j < 16; j++)
        {
            char memVal[5];
            sprintf(memVal, "%04hX", finished.mem[i*16 + j]);
            printf("%s ", memVal);
        }
        printf("\n");
    }
     
    
};

//================================================
// main function
//================================================



/**
 * Main function
 * Inputs: optional file path
 * What it will do:
 * the function will first load the data and then it will run it, then it will print it
 */
int main(int argc, char *argv[]){

    Hatchling emptyarg;
    Hatchling loaded = loadData(argc, argv, emptyarg);
    printf("\n*** PROGRAM LOADING COMPLETED ***\n");
    printf("*** PROGRAM EXECUTION BEGINS  ***\n");
    Hatchling finishedhml = runData(loaded);

    
    printData(finishedhml);

    return(0);
}



