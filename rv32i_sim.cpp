/*
This is just a skeleton. It DOES NOT implement all the requirements.
It only recognizes the "ADD", "SUB" and "ADDI"instructions and prints
"Unkown Instruction" for all other instructions!

References:
(1) The risc-v ISA Manual ver. 2.1 @ https://riscv.org/specifications/
(2) https://github.com/michaeljclark/riscv-meta/blob/master/meta/opcodes
*/

#include <iostream>
#include <fstream>
#include "stdlib.h"
#include <iomanip>
#include <string>
using namespace std;

int regs[32] = { 0 };
unsigned int pc = 0x0;

char memory[8 * 1024];	// only 8KB of memory located at address 0

void emitError(char *s)
{
	cout << s;
	exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW) {
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}
void instDecExec(unsigned int instWord)
{
	unsigned int rd, rs1, rs2, funct3, funct7, opcode;
	unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;

	unsigned int instPC = pc - 4;

	opcode = instWord & 0x0000007F;
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;
	funct7 = (instWord >> 25) & 0x0000007F;


	// — inst[31] — inst[30:25] inst[24:21] inst[20]
	I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));

	// concatenated the two immediate fields
	unsigned int temp1 = ((instWord >> 25) & 0x3F);
	unsigned int temp2 = ((instWord >> 7) & 0xF);
	char str[100];
	sprintf(str, "%d%d", temp2, temp1);
	S_imm = strtol(str, NULL, 10);

	unsigned int temp3 = ((instWord >> 8) & 0x7);
	unsigned int temp4 = ((instWord >> 25) & 0x1F);
	unsigned int temp5 = ((instWord >> 7) & 0x1);
	unsigned int temp6 = ((instWord >> 30) & 0x1);
	char str2[100];
	B_imm = sprintf(str, "%d%d", temp3, temp4);
	B_imm = sprintf(str, "%d%d", B_imm, temp5);
	B_imm = sprintf(str, "%d%d", B_imm, temp6);

	U_imm = ((instWord >> 12) & 0x7FFF);

	printPrefix(instPC, instWord);

	if (opcode == 0x33) {		// R Instructions
		switch (funct3) {
		case 0: if (funct7 == 32)
		{
			cout << "\tSUB\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			regs[rd] = regs[rs1] - regs[rs2];
		}
				else
				{
					cout << "\tADD\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
					regs[rd] = regs[rs1] + regs[rs2];
				}
		case 1:

			cout << "\tSLL\tx" << rd << ", x" << rs1 << ", x" << rs2   << "\n";
			regs[rd] = regs[rs1] << regs[rs2];
			break;


		case 2:
			cout << "\tSLT\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n"; // set on less than  if rs1< rs2, set rd to be equal 1
			if (regs[rs1] < regs[rs2])
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;


		case 3:
			cout << "\tSLLU\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			if ((unsigned int)regs[rs1] < (unsigned int)regs[rs2])
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;


		case 4:
			cout << "\tXOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			regs[rd] = regs[rs1] ^ regs[rs2];
			break;


		case 5:
			if (funct7 == 32)
			{
				cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] >> regs[rs2];   // 
			}
			else
			{
				cout << "\tSRA\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = (unsigned int)regs[rs1] >> regs[rs2];
			}


		case 6:
			cout << "\tOR\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			regs[rd] = regs[rs1] | regs[rs2];


		case 7:
			cout << "\tAND\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
			regs[rd] = regs[rs1] & regs[rs2];


		default:
			cout << "\tUnkown R Instruction \n";
		}
	}




	else if (opcode == 0x13)     // ask how is it 13
	{	// I instructions
		switch (funct3) {
		case 0:
			cout << "\tADDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] + (int)I_imm;
			break;

		case 1:
			cout << "\tSLLI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] << (int)I_imm;
			break;


		case 2:
			// places the value 1 in register rd if register rs1 is less than the signextended immediate when both are treated as signed numbers, else 0 is written to rd. 
			cout << "\tSLTI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] < (int)I_imm)
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;


		case 3:
			// SLTIU rd, rs1, 1 sets rd to 1 if rs1 equals zero, otherwise sets rd to 0  === seqz rd, rs 
			cout << "\tSLTIU\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] == 0)
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;


		case 4:
			cout << "\tXORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] ^ (int)I_imm;
			break;

		case 5:                    //// back to it later to be sure of that shamt thing and that it doesn't affect the value of func7
			if (funct7 == 32)
			{
				cout << "\tSRAI\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = regs[rs1] >> (int)I_imm;   // 
			}
			else
			{
				cout << "\tSRLI\tx" << rd << ", x" << rs1 << ", x" << rs2 << "\n";
				regs[rd] = (unsigned int)regs[rs1] >> (int)I_imm;;
			}


		case 6:
			cout << "\tORI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] | (int)I_imm;      // ask if s1 should be signed or unsigned 
			break;

		case 7:
			cout << "\tANDI\tx" << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] & (int)I_imm;      // ask if s1 should be signed or unsigned 
			break;
		default:
			cout << "\tUnkown I Instruction \n";
		}
	}

	else if (opcode == 0x3)
	{
		// The effective byte address is obtained by adding register rs1 to the sign-extended 12-bit offset. Loads copy a value from memory to register rd
		switch (funct3) {
		case 0:
			cout << "\tLB\tx" << rd << ", " << hex << "0x" << (int)I_imm << ", (x" << rs1 << ")" << "\n";
			regs[rd] = ((memory[regs[rs1] + (int)I_imm])) & 0x000000ff;                     // ask about it  also it should be sign extended but i don't know how now
			break;


		case 1:
			// LH loads a 16-bit value from memory, then sign-extends to 32-bits before storing in rd
			cout << "\tLH\tx" << rd << ", " << hex << "0x" << (int)I_imm << ", (x" << rs1 << ")" << "\n";
			regs[rd] = ((memory[regs[rs1] + (int)I_imm])) & 0x0000ffff;                     // ask about it
			break;

		case 2:
			cout << "\tLW\tx" << rd << ", " << hex << "0x" << (int)I_imm << ", (x" << rs1 << ")" << "\n";
			regs[rd] = ((memory[regs[rs1] + (int)I_imm]));                     // ask about it
			break;

		case 4:
			cout << "\tLBU\tx" << rd << ", " << hex << "0x" << (int)I_imm << ", (x" << rs1 << ")" << "\n";
			regs[rd] = (unsigned char)((memory[regs[rs1] + (int)I_imm])) & 0x000000ff;                     // ask about it
			break;

		case 5:
			cout << "\tLHU\tx" << rd << ", " << hex << "0x" << (int)I_imm << ", (x" << rs1 << ")" << "\n";
			regs[rd] = (unsigned char)((memory[regs[rs1] + (int)I_imm])) & 0x0000ffff;                     // ask about it
			break;
		}
	}


	else if (opcode == 0x23)
	{
		switch (funct3) {
		case 0:
			// The effective byte address is obtained by adding register rs1 to the sign-extended 12-bit offset.  Stores copy the value in register rs2 to memory.
			//The SW, SH, and SB instructions store 32-bit, 16-bit, and 8-bit values from the low bits of register rs2 to memory.

			cout << "\tSB\tx" << rs2 << ", " << hex << "0x" << (int)S_imm << ", (x" << rs1 << ")" << "\n";
			// some instruction
			break;


		case 1:
			cout << "\tSH\tx" << rs2 << ", " << hex << "0x" << (int)S_imm << ", (x" << rs1 << ")" << "\n";
			// some instruction
			break;


		case 2:
			cout << "\tSW\tx" << rs2 << ", " << hex << "0x" << (int)S_imm << ", (x" << rs1 << ")" << "\n";
			(memory[regs[rs2] + (int)S_imm]) = regs[rs1];
			break;

		}
	}
	else if (opcode == 0x63)
	{
		switch (funct3)
		{
		case 0:
			cout << "\tBEQ\tx" << rs1 << ", x" << rs2 << hex << "0x" << (int)B_imm; // ask how to reach the word that should replace B-immediate
			if (regs[rs1] == regs[rs2])
				pc = pc + (int)B_imm;
			break;
		case 1:
			cout << "\tBNE\tx" << rs1 << ", x" << rs2 << hex << "0x" << (int)B_imm;
			if (regs[rs1] != regs[rs2])
				pc = pc + (int)B_imm;
			break;
		case 4:
			cout << "\tBLT\tx" << rs1 << ", x" << rs2 << hex << "0x" << (int)B_imm;
			if (regs[rs1] < regs[rs2])
				pc = pc + (int)B_imm;
			break;
		case 5:
			cout << "\tBGE\tx" << rs1 << ", x" << rs2 << hex << "0x" << (int)B_imm;
			if (regs[rs1] >= regs[rs2])
				pc = pc + (int)B_imm;
			break;
		case 6:
			cout << "\tBLTU\tx" << rs1 << ", x" << rs2 << hex << "0x" << (int)B_imm;
			if (unsigned int(regs[rs1]) < unsigned int(regs[rs2]))
				pc = pc + (int)B_imm;
			break;
		case 7:
			cout << "\tBGEU\tx" << rs1 << ", x" << rs2 << hex << "0x" << (int)B_imm;
			if (unsigned int(regs[rs1]) >= unsigned int(regs[rs2]))
				pc = pc + (int)B_imm;
			break;

		}
	}




// LUI places the 20-bit U-immediate into bits 31–12 of register rd and places zero in the lowest 12 bits. The 32-bit result is sign-extended to 64 bits.
	else if (opcode == 0x37) 
	{

		cout << "\tLUI\tx" << rd << hex << "0x" << (int)U_imm;
		regs[rd] = (int)U_imm;
		regs[rd] = regs[rd] >> 12;
	}
// appends 12 low-order zero bits to the 20-bit U-immediate, sign-extends the result to 64 bits, adds it to the address of the AUIPC instruction, then 
//places the result in register rd.


	else if (opcode == 0x17)
	{
			cout << "\tAUIPC\tx" << rd << hex << "0x" << (int)U_imm;
			unsigned int store1 = (int)U_imm;
			store1 = store1 >> 12;
			regs[rd] = store1+pc;
		
	}


	else if (opcode == 0x73)      // ecall
	{
		cout << "ECALL ";
		switch (regs[17])
		{
		case 1:
			cout << regs[10];
			break;
		case 4:
			while (memory[regs[10]] != '/0')
			{
				cout << memory[regs[10]];
				regs[10]++;
			}
			break;

		case 5:
			cin >> regs[10];
			break;

		case 8:
			cin >> regs[11];
			int i = 0;
			while (i <= regs[11])
			{
				i++;
				cin >> memory[regs[10]];
			}


		}
	}

	else
	{
		cout << "\tUnkown Instruction \n";
	}

}

int main(int argc, char *argv[]) {

	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;

	if (argc<1) emitError("use: rv32i_sim <machine_code_file_name>\n");

	inFile.open(argv[1], ios::in | ios::binary | ios::ate);

	if (inFile.is_open())
	{
		int fsize = inFile.tellg();

		inFile.seekg(0, inFile.beg);
		if (!inFile.read((char *)memory, fsize)) emitError("Cannot read from input file\n");

		while (true) {
			instWord = (unsigned char)memory[pc] |
				(((unsigned char)memory[pc + 1]) << 8) |
				(((unsigned char)memory[pc + 2]) << 16) |
				(((unsigned char)memory[pc + 3]) << 24);
			pc += 4;
			// remove the following line once you have a complete simulator
			if (pc == 32) break;			// stop when PC reached address 32
			instDecExec(instWord);
		}

		// dump the registers
		for (int i = 0; i<32; i++)
			cout << "x" << dec << i << ": \t" << "0x" << hex << std::setfill('0') << std::setw(8) << regs[i] << "\n";

	}
	else emitError("Cannot access input file\n");
}
