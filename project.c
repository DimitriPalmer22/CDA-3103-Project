#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    /* Dimitri
     * Split the instruction into parts
     *
     * The opcode is always the first 6 bits
     *
     */

    // Create a bit mask to only read 5 bits
    const unsigned int bitMask5 = 0b00000000000000000000000000011111;

    // Create a mask to only take in 6 bits
    const unsigned int bitMask6 = 0b00000000000000000000000000111111;

    // Shift the instruction over to only get the last 6 bits & use the mask to get the bits
    *op = (instruction >> 26) & bitMask6;

    // TODO: Delete
    printf("INSTRUCTION: %d. OP: %d\n", instruction, *op);

    // Depending on the op code, we have to read different values from the instruction
    // If the op code is 000000, then the instruction is r-type
    if (*op == 0b000000) {
        // TODO: Delete
        printf("Instruction Type: R-Type\n");

        // Read in r1 (5 bits)
        *r1 = (instruction >> 21) & bitMask5;

        // Read in r2 (5 bits)
        *r2 = (instruction >> 16) & bitMask5;

        // Read in r3 (5 bits)
        *r3 = (instruction >> 11) & bitMask5;

        // Read in shamt (5 bits)
        // *r1 = (instruction >> 6) & bitMask5;

        // read in funct (6 bits)
        *funct = (instruction >> 0) & bitMask6;

        // TODO: Delete
        printf("### INSTRUCTION: %d %d %d %d __ %d", *op, *r1, *r2, *r3, *funct);

    }

    // J - type opcodes are 00001x
    else if ((*op >> 1) == 0b00001) {
        // TODO: Delete
        printf("Instruction Type: J-Type\n");

        // Read in the jump into jsec (26)
        *jsec = (instruction >> 0) & bitMask6;

        // TODO: Delete
        printf("### INSTRUCTION: %d %d", *op, *jsec);
    }

    // The other function types are coprocessor functions, which start with 0100xx
    else if ((*op >> 2) == 0b100) {
        printf("Instruction Type: Coprocessor\n");
    }

    // I-types are everything else
    else {
        // TODO: Delete
        printf("Instruction Type: I-Type\n");

        // Read in r1 (5 bits)
        *r1 = (instruction >> 21) & bitMask5;

        // Read in r2 (5 bits)
        *r2 = (instruction >> 16) & bitMask5;

        // Create a 16-bit bit mask
        const unsigned int bitMask16 = 0b00000000000000001111111111111111;

        // Read in immediate into offset (16)
        *offset = (instruction >> 0) & bitMask16;

        // TODO: Delete
        printf("### INSTRUCTION: %d %d %d %d", *op, *r1, *r2, *offset);
    }
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{

}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{

}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

