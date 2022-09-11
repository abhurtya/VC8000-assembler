# VC8000-assembler

This is an assembler and emulator for the VC8000 computer. 

# The VC8000 Computer

The VC8000 is a decimal computer with 1,000,000 words of memory. Each word consists of 9 decimal digits. There is an additional 10 words of memory called registers. These are on chip memory.  The machine language instructions for the VC8000 is of the following form  Note: each has the same number of digits.

 

                             |2 digits             | 1 digit                 |6 digits       |
                             |operation code       | register number         |address portion|  

                                                      or

                             |2 digits             | 1 digit                 |1 digit               |  5 digits     |
                             |operation code       | register number         |register number       | unused        |

 A machine language program is a sequence of machine language instructions stored in memory. The computer runs a machine language program by executing machine instructions stored in successive words of memory. The VC8000 machine assumes that the first instruction to be executed will be at location 100. The following are the machine language instructions for the VC8000.

NAME           OP. CODE     MEANING

ADD                     01           Reg <-- c(Reg) + c(ADDR)   (The contents of the register specified in the instruction and of the memory location specified by the address portion of the instruction are added together. The result is placed in the register.):

SUBTRACT         02            Reg <-- c(Reg) - c(ADDR)

MULTIPLY           03            Reg <-- c(Reg) * c(ADDR)

DIVIDE                 04            Reg <-- c(Reg) / c(ADDR)

LOAD                   05            Reg <-- c(ADDR)

STORE                06            ADDR <-- c(Reg)

ADD REG            07            REG1 <--c(REG1) + c(REG2)   (Note: this is the second instruction format where two registers are specified.)

SUB REG            08            REG1 <--c(REG1) - c(REG2)  

MULT REG          09            REG1 <--c(REG1) * c(REG2)  

DIV REG              10            REG1 <--c(REG1) / c(REG2)  

READ                   11            A line is read in and the number found there is recorded in the specified memory address.  The register value is ignored.

WRITE                 12            c(ADDR) is displayed  The register value is ignored.

BRANCH              13           go to ADDR for next instruction.  The register value is ignored.

BRANCH MINUS  14          go to ADDR if c(Reg) < 0

BRANCH ZERO    15          go to ADDR if c(Reg) = 0

BRANCH POSITIVE 16       go to ADDR if c(Reg) > 0

HALT                    17           terminate execution.  The register value and address are ignored.

We will write the following examples programs in class to get a feeling for this computer:

Print the value of C(5)+C(300)
Print C(200)*C(6)+C(300)*C(7)
Read in a number and display its factorial.
Read in 100 nos. and print their average.
