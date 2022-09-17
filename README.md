# Hatchling: A Simple Computer Emulator
### This project is due September 16 by 23:59:59.


## Machine Language Programming
Let us create a computer we will call Hatchling.  As the name implies, it is a small, simple machine, but as you will see, a very powerful one as well.  The Hatchling runs programs written in the only language it understands, Hatchling Machine Language, or HML for short.

The Hatchling contains an __accumulator__ -- a "special register" in which information is put before the Hatchling uses that information in calculations or examines it in various ways.  All instructions are a four-digit hexadecimal number such as `4004`, `4106`, `FF00`, `001E`, etc.  Each hexadecimal number is a 16-bit integer (in the C language that is a `short int` or an `unsigned short int`).  Hatchling is equipped with a 256 word memory, and these words are referenced by their memory location numbers (address) `00`, `01`, ... ,`FF`.

Before running an HML program, we must __load__ or place the program into memory.  The first instruction (or statement) of every HML program is always placed in location `00`.

Each instruction written in HML occupies one word of Hatchling's memory (and hence instructions are four-digit hexadecimal numbers).  We shall assume that the sign of an HML instruction word is always non-negative, but the sign of a data word may be signed according to rules of 2's complement integers.  The first two digits of each HML instruction are the __operation code__, which specifies the operation to be performed.  HML operation codes are shown below.

### HML OP CODES
```c
// ACC-MEM Arithmetic Instructions
#define ADD     0x10    // Add a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define SUB     0x11    // Subtract a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define MUL     0x12    // Multiple a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define DIV     0x13    // Divide a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)
#define MOD     0x14    // Compute the integer remainder when dividing a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator)

// ACC-MEM Logical Instructions
#define AND     0x20    // Compute the binary AND of a specific memory location and the accumulator (leave the result in the accumulator)
#define ORR     0x21    // Compute the binary OR of a specific memory location and the accumulator (leave the result in the accumulator)
#define NOT     0x22    // Compute the logical NOT of the accumulator (leave the result in the accumulator)
#define XOR     0x23    // Compute the binary XOR of a specific memory location and the accumulator (leave the result in the accumulator)
#define LSR     0x24    // Compute the binary logical shift right (1-bit) of the accumulator (leave the result in the accumulator)
#define ASR     0x25    // Compute the binary arithmetic shift right (1-bit) of the accumulator (leave the result in the accumulator)
#define LSL     0x26    // Compute the binary logical shift left of the accumulator (leave the result in the accumulator)

// Branch Instructions
#define B       0X30    // Branch to a specific location in memory
#define BNEG    0X31    // Branch to a specific location in memory if the accumulator is negative
#define BPOS    0X32    // Branch to a specific location in memory if the accumulator is positive
#define BZRO    0X33    // Branch to a specific location in memory if the accumulator is zero

// ACC-MEM Load/Store Instructions
#define LOAD    0X40    // Load a word from a specific location in memory into the accumulator
#define STOR    0X41    // Store a word from the accumulator into a specific memory location

// I/O (Standard) Instructions
#define READ    0X50    // Read a word from the terminal into a specific location in memory
#define WRTE    0X51    // Write a word from a specific location in memory to the terminal

// HALT Instruction
#define HALT    0XFF    // Halt, i.e. the program has completed its task

```

The last two digits of an HML instruction are the __operand__, which is the address of the memory location containing the word to which the operation applies.

### Example HML Program 1
```text
00  4004    LOAD A 
01  1005    ADD B
02  4106    STOR C
03  FF00    HALT
04  00ff    Variable A
05  001e    Variable B
```
This HML program takes the value at Mem[04] and loads it into the accumulator.  Then it adds the value at Mem[05] to the accumulator.  Then it stores the result into Mem[06].  Halt is the last instruction.

### Example HML Program 2
```text
00  5009    READ A
01  500A    READ B
02  4009    LOAD A
03  110A    SUB B
04  3107    BNEG to 07
05  5109    WRTE A
06  FF00    HALT
07  510A    WRTE B
08  FF00    HALT
09  0000    VARIABLE A
0A  0000    VARIABLE B
```
This example program reads two integers and prints the larger of the two.


## PART 1 (15 points)
Now write HML programs to accomplish each of the following tasks.
1. (5 points) Use a sentinel-controlled loop to read 10 positive numbers and compute and print their sum. (one.hml)
2. (5 points) Use a counter-controlled loop to read seven numbers, some positive, some negative, and compute and print their average. (two.hml)
3. (5 points) Read a series of numbers and determine and print the largest number.  The first number read indicates how many numbers should be processed. (three.hml)


## Hatchling Simulator (85 points)
Write a C program that implements a Hatchling simulator.  Your simulator will read an HML program from either standard input or a file.  Output from your simulator will be to standard output (terminal).  Your program needs to be able to read an HML program from either standard input (sentinel value -99999 is marks the end of the HML program) or from a file whose name is passed in as a command line argument.  

```text
linux$ ./hmlsim [ENTER]
00  4004
01  1005
02  4106
03  FF00
04  00FF
05  001E
06  011D
0B  -99999
*** PROGRAM LOADING COMPLETED ***
*** PROGRAM EXECUTION BEGINS  ***
*** PROGRAM EXECUTION TERMINATED ***

REGISTERS
ACC     011D
InstCtr   04
InstReg FF00
OpCode    FF
Operand   00

MEMORY:
           0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
   0	4004  1005  4106  FF00  00FF  001E  011D  0000  0000  0000  0000  0000  0000  0000  0000  0000
  10	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  20	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  30	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  40	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  50	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  60	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  70	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  80	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  90	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  A0	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  B0	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  C0	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  D0	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  E0	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  F0	0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000  0000
  linux$
  linux$ ./hmlsim one.hml
  <output is similar>
  linux$
```

Once your HML program is in memory, your simulator will begin execution of your HML program.  Execution begins with the instruction at memory location `00` and like all other computers, continues sequentially until the `HALT` instruction is executed.

Your program will move the instruction in memory at the InstCtr location into the instruction register.  Your program will then parse/divide/determine the op-code and the operand.  I use a large `switch` statement in C using the op-code to determine what operation to perform.  

__Use documentation in the op-code section above to determine what to do for each operation.__

## Error Checking
Your simulator should check for various types of errors.  During the program loading phase, each number (instruction) must be in the range `[0000..FFFF]`.  If not, your program should output the message `"BAD INSTRUCTION ON LINE XX"` where XX is the memory address / line number where the bad instruction was detected.

During the execution phase, your simulator should check for various serious errors, such as attempts to divide by zero, attempts to execute bad op-codes, and accumulator overflows.  Such errors are called __fatal errors__.  When a fatal error is detected, your simulator should print an error message such as:

```text
*** ATTEMPT TO DIVIDE BY ZERO ***
*** HATCHLING EXECUTION ABNORMALLY TERMINATED ***
```
and should print a full computer dump in the format shown above.  This will help the user locate the error in the program.

## Notes / Helpful Tips
1.  This is an individual assignment worth 100 points.
2.  This project is due September 16 by 23:59:59.
3.  Your program will be tested on `babbage.cs.tcu.edu` with several example programs.
4.  This assignment is worth 100 points.
5.  Your program must be written in C (not C++).
6.  Use good structured program design in your solution.  You must write several C functions to receive full credit.
7.  You must document your program.  Each function should start with a document block.  Also, document each major step in your function. 
8.  Use GitHub and GitHub Classroom to keep your program saved and updated in its repository.  Final submission will be using GitHub Classroom. 
9.  The use of global variables is forbidden in this assignment.  You will have to pass variables to functions either by value or by reference.
10. You will need to implement the following variables/fields in your simulator.  I suggest that you store these in a `struct` that way it will be easy to pass to functions.
    1.  Accumulator - used to represent the accumulator register
    2.  Instruction Counter - used to keep track  of the location in memory that contains the instruction being performed.
    3.  Instruction Register - a variable used store the instruction being performed.  `Instruction Register = Mem[Instruction Counter]`
    4.  Operation Code - upper byte of the instruction register (>> 8)
    5.  Operand - lower byte of the instruction register (& 0xFF)
    6.  Memory - an array of 256 `short int` 
11. All of the above values are initialized to zero just before execution begins.
12. The input format string `"%hX"` may be helpful for reading hexadecimal values from input.
13. The output format strings `"%4hX"` and `"%04hX"` may be helpful in printing `short int` values to output.

