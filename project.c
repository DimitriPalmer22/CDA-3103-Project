#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) {
	/*
		Christopher and Isabel
		A and B are in 2s complement unless specified as unsigned
		ALUControl is represented with chars '0' - '7'
	*/

	//use to ensure carry out bit is ignored
	const unsigned int bitMask32 = 0b11111111111111111111111111111111;

	switch(ALUControl) {
		case '0': //add
			*ALUresult = A + B;
			*ALUresult = *ALUresult & bitMask32;
			break;
		case '1': //subtract
			if(B != 0)
				B = ~B + 1;
			*ALUresult = A + B;
			*ALUresult = *ALUresult & bitMask32;
			break;
		case '2': //(A < B)?
			if(A >> 31 == 1 && B >> 31 == 0) //A is -, B is +
				ALUresult = 1;
			else if(A >> 31 == 0 && B >> 31 == 1) //A is +, B is -
				ALUresult = 0;
			else if(A >> 31 == 1 && B >> 31 == 1) //A is -, B is -
				//if A has a greater magnitude then it's smaller
				ALUresult = A > B;
			else //A is +, B is +
				//if A has a smaller magnitude then it's smaller
				ALUresult = A < B;
			break;
		case '3': //(A < B)? (A and B are unsigned integers)
			*ALUresult = A < B;
			break;
		case '4': //(A AND B)
			*ALUresult = A & B;
			break;
		case '5': //(A OR B)
			*ALUresult = A | B;
			break;
		case '6': //(Shift B left by 16 bits)
			*ALUresult = B << 16;
			break;
		case '7': //(NOT A)
			*ALUresult = ~A;
			break;
	}

    *ALUresult == 0 ? (*Zero = '1') : (*Zero = '0'); //set Zero flag to appropriate value
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    // Check word alignment
    if (PC%4 == 0)
    {
        // Shift
        unsigned tmp = PC >> 2;
        // Check if Mem contains a value
        if (Mem[tmp] == NULL)
        {
            return 1;
        }
        *instruction = Mem[tmp];
        return 0;
    }
    else
    {
        return 1;
    }
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
	printf("I got the op code %d!\n", *op);

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
        printf("### INSTRUCTION: %d %d %d %d __ %d\n", *op, *r1, *r2, *r3, *funct);

    }

    // J - type opcodes are 00001x
    else if ((*op >> 1) == 0b00001) {
        // TODO: Delete
        printf("Instruction Type: J-Type\n");

        // Read in the jump into jsec (26)
        *jsec = (instruction >> 0) & bitMask6;

        // TODO: Delete
        printf("### INSTRUCTION: %d %d\n", *op, *jsec);
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
        printf("### INSTRUCTION: %d %d %d %d\n", *op, *r1, *r2, *offset);
    }
}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{
    /*
        TODO Maybe - the components of the controls struct are characters so
        when we're setting a component of controls equal to something it has
        to be a character. For example, look at the ALUOp codes in ALU function.
        They are '0' ... '7'. I think this applies to everything here.
    */
    printf("Potential TODO above in code!\n");

    printf("DECODE FUNCTION: %d\n", op);

	// Set all controls to 0 by default, so I don't have to set everything later
	controls->RegDst = 0;
	controls->ALUSrc = 0;
	controls->MemtoReg = 0;
	controls->RegWrite = 0;
	controls->MemRead = 0;
	controls->MemWrite = 0;
	controls->Branch = 0;
	controls->Jump = 0;
	controls->ALUOp = 0;

    switch (op)
    {
    // R-Format instruction
    // add
    // sub
    // and
    // or
    // slt
    // sltu
    case 0b000000:
        printf("DECODE: R-Type!\n");

        controls->RegDst = 1;
        controls->RegWrite = 1;
    	controls->ALUOp = 0b111;
		break;

    // LW
    case 0b100011:
        printf("DECODE: LW!\n");

        controls->ALUSrc = 1;
        controls->MemtoReg = 1;
        controls->RegWrite = 1;
        controls->MemRead = 1;
    	controls->ALUOp = 0b000;
		break;

    // SW
    case 0b101011:
        printf("DECODE: SW!\n");

        controls->RegDst = 2;
        controls->ALUSrc = 1;
        controls->MemtoReg = 2;
        controls->MemWrite = 1;
    	controls->ALUOp = 0b000;
        break;

    // BEQ
    case 0b000100:
        printf("DECODE: BEQ!\n");

        controls->RegDst = 2;
        controls->MemtoReg = 2;
        controls->Branch = 1;
    	controls->ALUOp = 0b001;
    	break;

    // addi
	case 0b001000:
    	printf("DECODE: addi!\n");

    	controls->ALUSrc = 1;
    	controls->RegWrite = 1;
    	controls->ALUOp = 0b000;
		break;

    // lui
    case 0b001111:
    	printf("DECODE: lui!\n");

    	controls->ALUSrc = 1;
    	controls->RegWrite = 1;
    	controls->ALUOp = 0b110;
    	break;

    // slti
    case 0b001010:
    	printf("DECODE: slti!\n");

    	controls->ALUSrc = 1;
    	controls->RegWrite = 1;
    	controls->ALUOp = 0b010;
    	break;

    // sltiu
    case 0b001011:
    	printf("DECODE: sltiu!\n");

    	controls->ALUSrc = 1;
    	controls->RegWrite = 1;
    	controls->ALUOp = 0b011;
    	break;

    // jump
    case 0b000010:
    	printf("DECODE: jump!\n");

    	controls->Jump = 1;
    	controls->ALUOp = 0b000;
    	break;

    // Return -1 if a halt occurs.
    // Something like if the instruction is invalid
    default:
        printf("DECODE INVALID!\n");
        return -1;
    }

    // Return a 0 if halt does NOT occur.
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    /*
        Index into Reg at r1 and r2 and write the read
        values into data 1 and data 2 respectively.
    */
    Reg[r1] = *data1;
    Reg[r2] = *data2;
}


/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    //bit masks
    const unsigned int bitMask32 = 0b11111111111111111111111111111111;

    if(offset >> 15 == 0) { //if positive
        //fill with zeros
        offset = offset & bitMask32;
    } else { //if negative
        //flip to positive
        offset = ~offset + 1;

        //fill with ones
        offset = offset & bitMask32;

        //flip back to negative
        offset = ~offset + 1;
    }

    //set extended value
    *extended_value = offset;
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
//Check if R type instruction, if so check funct and update instruction
if (ALUOp == 7){
    if(funct == 4){
        ALUOp = 6;
    }
    else if (funct == 32){
        ALUOp = 0;
    }
    else if (funct == 34){
        ALUOp = 1;
    }
    else if (funct == 36){
        ALUOp = 4;
    }
    else if (funct == 37){
        ALUOp = 5;
    }
    else if (funct == 39){
        ALUOp = 7;
    }
    else if (funct == 42){
        ALUOp = 2;
    }
    else if (funct == 43){
        ALUOp = 3;
    }
    //If cannot be changed from R type instruction, return 1 as a "fail"
    else{
        return 1;
    }
}
//Check if second data is supposed to be itself or the extended value
if (ALUSrc == 1){
    data2 = extended_value;
}

//Call ALU with updated values and instructions
ALU(data1,data2,ALUOp,ALUresult,Zero);

return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    /*
        Chris and Isabel
        We were under the following understandings when writing this:
            - MemWrite or MemRead is '1' (unless they are both '0')
            - It is not possible for both MemWrite and MemRead to be '1'
            - if both MemWrite or MemRead is '0' we do nothing
    */
    if(MemWrite == '1' || MemRead == '1') { //checks if we're doing anything here
        if(ALUresult % 4 == 0 && ALUresult >= 0 && (ALUresult >> 2) <= (16384 - 1)) { //word alligned? address valid?
            if(MemWrite == '1') { //writing to memory
                Mem[ALUresult >> 2] = data2;
            } else { //reading from memory
                *memdata = Mem[ALUresult >> 2];
            }
        } else { //ALUresult isn't a valid address
            return 1;
        }
    }

    return 0; //no halt
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
	/*
	 * Dimitri
	 *
	 * As far as I know, this function is supposed to write a value
	 * into a register either from memory from the output of the ALU.
	 *
	 * r2 and r3 are the possible indices of the register we are writing to
	 *
	 * memdata is a value that is currently stored in memory
	 * ALUresult is the value that is being outputted from the ALU
	 *
	 * RegWrite, RegDst, and MemtoReg are flags set by instruction_decode
	 *
	 * Reg is the array of register values (32 total)
	 *
	 */

	// Depending on the flags, we
	// - determine which value is being written to the register
	// - determine which register is being written to

	// If the RegWrite flag is not set, then do not do anything
	if (RegWrite == 0)
		return;

	// Determine which register index we are using
	unsigned registerIndex = r2;

	// Determine which value is being stored into the register
	unsigned value = memdata;

	// The value is coming from memory
	// Load word
	if (MemtoReg == 1)
	{
		registerIndex = r2;
		value = memdata;
	}

	// The value is coming from the ALU result
	else if (MemtoReg == 0)
	{
		value = ALUresult;

		// Change the register index based on the RegDst flag

		// if RegDst == 1, then the instruction is r-type
		// so, we need to write to register 3
		if (RegDst == 1)
			registerIndex = r3;

		// if RegDst == 0, then the instruction is one of those immediate instructions
		// So, we need to write to register 2
		else
			registerIndex = r2;
	}

	// If the register index is 0, return
	// We cannot write to register index 0 since it is $zero
	// Idk if the program will ever even ask us to write to $zero,
	// but this is here just in case
	if (registerIndex == 0)
		return;

	// Set the register value
	Reg[registerIndex] = value;

	printf("In WriteRegister. {%d & %d}. {%d & %d}\n", r2, r3, memdata, ALUresult);
	printf("Write %d to Reg[%d]!\n", value, registerIndex);
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{

}

