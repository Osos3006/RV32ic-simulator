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
using namespace std ;
bool compressed = false;
bool flag = true;
int regs[32] = { 0 };
unsigned int pc = 0x0;

char memory[8 *1024] = {'0'};// only 8KB of memory located at address 0

void emitError(char*s)
{
	cout << s;
	exit(0);
}

void printPrefix(unsigned int instA, unsigned int instW) {
	cout << "0x" << hex << std::setfill('0') << std::setw(8) << instA << "\t0x" << std::setw(8) << instW;
}
void instDecExec(unsigned int instWord)
{
	// for ( int i = 0 ; i <32 ; i++)
	// regs[i]=0;
	unsigned int rd, rs1, rs2, funct3, funct7, opcode;                                     
	unsigned int I_imm, S_imm, B_imm, U_imm, J_imm;
	unsigned int address;
	regs[0]=0;
	unsigned int instPC ;
    if ( compressed == false)
    {

    instPC = pc - 4;

	opcode = instWord & 0x0000007F;
	rd = (instWord >> 7) & 0x0000001F;
	funct3 = (instWord >> 12) & 0x00000007;
	rs1 = (instWord >> 15) & 0x0000001F;
	rs2 = (instWord >> 20) & 0x0000001F;
	funct7 = (instWord >> 25) & 0x0000007F;


	// — inst[31] — inst[30:25] inst[24:21] inst[20]
	I_imm = ((instWord >> 20) & 0x7FF) | (((instWord >> 31) ? 0xFFFFF800 : 0x0));
	U_imm = (instWord & 0xFFFFF000);


	S_imm = ((( instWord >> 20) & 0x7E0) | ((instWord >> 7)& 0x1F)) | ((instWord >> 31) ? 0xFFFFF800 : 0x0);
	
	B_imm = ((instWord >> 7) & 0x1E) | ((instWord >> 20) & 0x7E0) | ((instWord << 4)  & 0x800) | ((instWord >> 31) ? 0xFFFFF000 : 0x0);
	//J_imm = ((instWord >> 20)& 0x7FE) | ((instWord >> 9 )& 0x800)  | (instWord & 0xFF0000)| ((instWord >> 31) ? 0xFFF00000 : 0x0);
  J_imm = ((instWord & 0x7FE00000) >> 20) | ((instWord >> 20 & 0x1) << 11) | ((instWord >> 12 & 0x7F) << 12)  | (((instWord >> 31) ? 0xFFF80000 : 0x0));
	 //concatenated the two immediate fields
	// unsigned int temp1 = ((instWord >> 25) & 0x3F);
	// unsigned int temp2 = ((instWord >> 7) & 0xF);
	// char str[100];
	// sprintf(str, "%d%d", temp2, temp1);
	// S_imm = strtol(str, NULL, 10);

	// unsigned int temp3 = ((instWord >> 8) & 0x7);
	// unsigned int temp4 = ((instWord >> 25) & 0x1F);
	// unsigned int temp5 = ((instWord >> 7) & 0x1);
	// unsigned int temp6 = ((instWord >> 30) & 0x1);
	// char str2[100];
	// B_imm = sprintf(str2, "%d%d", temp3, temp4);
	// B_imm = sprintf(str2, "%d%d", B_imm, temp5);
	// B_imm = sprintf(str2, "%d%d", B_imm, temp6);
	// B_imm = strtol(str2, NULL, 10);

	// U_imm = ((instWord >> 12) & 0x7FFF);

	// temp3 = ((instWord >> 21) & 0x3FF);
	// temp4 = ((instWord >> 20) & 0x1);
	// temp5 = ((instWord >> 12) & 0xFF);
	// temp6 = ((instWord >> 31) & 0x1);
	// char str3[100];
	// J_imm = sprintf(str3, "%d%d", temp3, temp4);
	// J_imm = sprintf(str3, "%d%d", B_imm, temp5);
	// J_imm = sprintf(str3, "%d%d", B_imm, temp6);
	// J_imm = strtol(str3, NULL, 10);


	printPrefix(instPC, instWord);
	if ( instWord == 0 )
	{
		flag = true; 
		return ; 
	}

	if (opcode == 0x33) {		// R Instructions
		switch (funct3) {
		case 0: 
		if (funct7 == 32)
		{
			cout << "\tSUB\tx" << dec << rd << ", x" <<  dec << rs1 << ", x" << dec << rs2 << "\n";
			regs[rd] = regs[rs1] - regs[rs2];
			
		}
		else
		{
					cout << "\tADD\tx" << dec << rd << ", x" << dec <<rs1 << ", x" << dec <<rs2 << "\n";
					regs[rd] = regs[rs1] + regs[rs2];
		}
		break;
		case 1:

			cout << "\tSLL\tx" << dec <<rd << ", x" << dec <<rs1 << ", x" <<dec << rs2   << "\n";
			regs[rd] = regs[rs1] << regs[rs2];
			break;


		case 2:
			cout << "\tSLT\tx" << dec <<rd << ", x" << dec <<rs1 << ", x" << dec <<rs2 << "\n"; // set on less than  if rs1< rs2, set rd to be equal 1
			if (regs[rs1] < regs[rs2])
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;


		case 3:
			cout << "\tSLTU\tx" << dec << rd << ", x" << dec << rs1 << ", x" << dec << rs2 << "\n";
			if ((unsigned int)(regs[rs1]) < (unsigned int)(regs[rs2]))
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;


		case 4:
			cout << "\tXOR\tx" << dec << rd << ", x" << dec <<rs1 << ", x" << dec <<rs2 << "\n";
			regs[rd] = regs[rs1] ^ regs[rs2];
			break;


		case 5:
			if (funct7 == 32)
			{
				cout << "\tSRA\tx" << dec << rd << ", x" << dec <<rs1 << ", x" <<dec << rs2 << "\n";
				regs[rd] = regs[rs1] >> regs[rs2];   // 
			}
			else
			{
				cout << "\tSRL\tx" << dec <<rd << ", x" <<dec << rs1 << ", x" <<dec << rs2 << "\n";
				regs[rd] = (unsigned int)regs[rs1] >> regs[rs2];
			}


		case 6:
			cout << "\tOR\tx" <<dec << rd << ", x" <<dec << rs1 << ", x" << dec <<rs2 << "\n";
			regs[rd] = regs[rs1] | regs[rs2];


		case 7:
			cout << "\tAND\tx" <<dec << rd << ", x" << dec <<rs1 << ", x" <<dec << rs2 << "\n";
			regs[rd] = regs[rs1] & regs[rs2];


		default:
			cout << "\tUnkown R Instruction \n";
		}
	}




	else if (opcode == 0x13)     // ask how is it 13
	{	// I instructions
		switch (funct3) {
		case 0:
			cout << "\tADDI\tx" << dec << rd << ", x" << dec <<rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] + (int)I_imm;
			break;

		case 1:
			cout << "\tSLLI\tx" << dec << rd << ", x" <<dec << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] << (int)I_imm;
			break;


		case 2:
			// places the value 1 in register rd if register rs1 is less than the signextended immediate when both are treated as signed numbers, else 0 is written to rd. 
			cout << "\tSLTI\tx" << dec << rd << ", x" << dec << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] < (int)I_imm)
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;


		case 3:
			// SLTIU rd, rs1, 1 sets rd to 1 if rs1 equals zero, otherwise sets rd to 0  === seqz rd, rs 
			cout << "\tSLTIU\tx" << dec << rd << ", x" <<dec << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			if (regs[rs1] == 0)
				regs[rd] = 1;
			else
				regs[rd] = 0;
			break;


		case 4:
			cout << "\tXORI\tx" << dec << rd << ", x" <<dec <<rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] ^ (int)I_imm;
			break;

		case 5:                    //// back to it later to be sure of that shamt thing and that it doesn't affect the value of func7
			if (funct7 == 32)
			{
				cout << "\tSRAI\tx" << dec << rd << ", x" << dec <<rs1 << ", x" <<dec << rs2 << "\n";
				regs[rd] = regs[rs1] >> (int)I_imm;   // 
			}
			else
			{
				cout << "\tSRLI\tx" << dec <<rd << ", x" << dec <<rs1 << ", x" << dec <<rs2 << "\n";
				regs[rd] = (unsigned int)regs[rs1] >> (int)I_imm;;
			}


		case 6:
			cout << "\tORI\tx" << dec <<rd << ", x" <<dec << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
			regs[rd] = regs[rs1] | (int)I_imm;      // ask if s1 should be signed or unsigned 
			break;

		case 7:
			cout << "\tANDI\tx" <<dec << rd << ", x" <<dec << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
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
			cout << "\tLB\tx" <<dec << rd << ", " << dec << (int)I_imm << " (x" << dec <<rs1 << ")" << "\n";
			regs[rd] = ((memory[regs[rs1] + (int)I_imm])) & 0x000000ff;                     // ask about it  also it should be sign extended but i don't know how now
			break;


		case 1:
			// LH loads a 16-bit value from memory, then sign-extends to 32-bits before storing in rd
			cout << "\tLH\tx" << dec << rd << ", " << dec << (int)I_imm << " (x" << dec << rs1 << ")" << "\n";
			regs[rd] = ((memory[regs[rs1] + (int)I_imm])) & 0x0000ffff;                     // ask about it
			break;

		case 2:
			cout << "\tLW\tx" << dec << rd << ", " << dec << (int)I_imm << " (x" << dec << rs1 << ")" << "\n";
			regs[rd] = ((memory[regs[rs1] + (int)I_imm]));                     // ask about it
			break;

		case 4:
			cout << "\tLBU\tx" << dec << rd << ", " << dec  << (int)I_imm << " (x" << dec <<rs1 << ")" << "\n";
			regs[rd] = (unsigned char)((memory[regs[rs1] + (int)I_imm])) & 0x000000ff;                     // ask about it
			break;

		case 5:
			cout << "\tLHU\tx" << dec << rd << ", " << dec << (int)I_imm << " (x" << dec <<rs1 << ")" << "\n";
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

			cout << "\tSB\tx" << dec << rs2 << ", " << dec << (int)S_imm << " (x" << dec << rs1 << ")" << "\n";
			memory[regs[rs1] + (int)S_imm] = regs[rs2] & 0x000000FF;
			break;


		case 1:
			cout << "\tSH\tx" <<dec << rs2 << ", " << dec << (int)S_imm << " (x" <<dec << rs1 << ")" << "\n";
			memory[regs[rs1] + (int)S_imm] = regs[rs2] & 0x0000FFFF;
			// some instruction
			break;


		case 2:
			cout << "\tSW\tx" << dec << rs2 << ", " << dec  << (int)S_imm << " (x" << dec << rs1 << ")" << "\n";
			(memory[regs[rs1] + (int)S_imm]) = regs[rs2];
			break;

		}
	}
	else if (opcode == 0x63)
	{
		switch (funct3)
		{
		case 0:
			cout << "\tBEQ\tx" << dec << rs1 << ", x" << dec << rs2 << ", " << hex << "0x" << (int)B_imm << "\n";// ask how to reach the word that should replace B-immediate
			if (regs[rs1] == regs[rs2])
				pc = pc + (int)B_imm  - 4;
			break;
		case 1:
			cout << "\tBNE\tx" << dec << rs1 << ", x" << dec <<rs2  << ", "<< hex << "0x" << (int)B_imm << "\n";
			if (regs[rs1] != regs[rs2])
				pc = pc + (int)B_imm  - 4;
			break;
		case 4:
			cout << "\tBLT\tx" << dec <<rs1 << ", x" << dec << rs2  << ", "<< hex << "0x" << (int)B_imm << "\n";
			if (regs[rs1] < regs[rs2])
				pc = pc + (int)B_imm  - 4;
			break;
		case 5:
			cout << "\tBGE\tx" << dec <<rs1 << ", x" << dec <<rs2  << ", " << hex << "0x" << (int)B_imm << "\n";
			if (regs[rs1] >= regs[rs2])
				pc = pc + (int)B_imm  - 4;
			break;
		case 6:
			cout << "\tBLTU\tx" << dec << rs1 << ", x" << dec <<rs2  << ", "<< hex << "0x" << (int)B_imm << "\n";
			if ((unsigned int)(regs[rs1]) < (unsigned int)(regs[rs2]))
				pc = pc + (int)B_imm  - 4;
			break;
		case 7:
			cout << "\tBGEU\tx" <<dec << rs1 << ", x" << dec <<rs2  << ", " << hex << "0x" << (int)B_imm << "\n";
			if ((unsigned int)(regs[rs1]) >= (unsigned int)(regs[rs2]))
				pc = pc + (int)B_imm  - 4;
			break;
		default:
		cout << " wrong branch instruction or func3 has a wrong number" << '\n' ;

		}
	}




// LUI places the 20-bit U-immediate into bits 31–12 of register rd and places zero in the lowest 12 bits. The 32-bit result is sign-extended to 64 bits.
	else if (opcode == 0x37) 
	{

		cout << "\tLUI\tx" << dec <<rd << hex << "0x" << (int)U_imm;
		regs[rd] = (int)U_imm;
		regs[rd] = regs[rd] >> 12;
	}
// appends 12 low-order zero bits to the 20-bit U-immediate, sign-extends the result to 64 bits, adds it to the address of the AUIPC instruction, then 
//places the result in register rd.


	else if (opcode == 0x17)
	{
			cout << "\tAUIPC\tx" << rd << " , " << hex << "0x" << ((int)U_imm >> 12) << endl;
			regs[rd] = (U_imm + (pc-4));
			
		
	}
	else 
		if (opcode == 0x6F)
		// J instructions
	// JAL instruction:
		{
		cout << "\tJAL\tx" << dec << rd << ", "<< hex << "0x" << (int)J_imm << '\n';

		regs[rd]= pc;
		pc += (int)J_imm -	4 ;

		}
	else 
			if (opcode == 0x67)
				//JALR instruction and it uses I-immediate
			{
				cout << "\tJALR\tx" << dec << rd << ", x" << rs1 << ", " << hex << "0x" << (int)I_imm << "\n";
				regs[rd] = pc ;
				pc = (regs[rs1] + int(I_imm)) & 0xFFFFFFFE; //adds rs1 to immediate and sets rightmost bit 0
			}

	

			
	else if (opcode == 0x73)      // ecall
	{
				int t = regs[10]; // %100 to test files from RARs, because the range of memory used for labels in Rars is far greater than the memory size in this simulation
				int i = 0;
				string t_str;
		cout << "\tecall\t" <<'\n';
		switch (regs[17])
		{
		case 1:
			cout << dec <<regs[10] << endl;
			break;
		case 4:
			
			while (memory[t] != '\0')
					{
						cout << memory[t];
						t++;
					}
			cout << endl;
			break;

		case 5:
			cin >> regs[10];
			break;

		case 8:
			//cin >> regs[11];
			//the ecall reads the string and store it as a null terminated string in memory
			t = regs[10];
					cin >> t_str;
					t_str += '\0'; //adds the null character to the string, however 
					while (i <= regs[11] && i < t_str.length())
					{
						
						memory[t] = t_str[i];
						t++;
						i++;
					}
					break;
		case 10:
			cout << "Program terminated" << endl;
			flag = false;
			return;
		}
	}

	else
	{
		cout << "\tUnkown Instruction \n";
	}

}
else
 {
  /*unsigned int rd, rs1, rs2, funct3, funct7, opcode;*/
    instPC = pc - 2;
  	printPrefix(instPC, instWord);
  unsigned int Uimm,immQ1,nzimm,Bimm, nzuimm , nzimm_addi16sp, imm, Jimm ;
 unsigned int add14imm ;
  add14imm = ((((instWord >> 6) & 0x0001) | ((instWord >> 4) & 0x0002) | ((instWord >> 9) & 0x000c) | ((instWord >> 3) & 0x00f0)) << 2);
   unsigned int lwspimm, swspimm;
  lwspimm = ((((instWord >> 4) & 0x0007) | ((instWord >> 9) & 0x0008) | ((instWord << 2) & 0x0030) )<< 2);
  swspimm = ((((instWord >> 9) & 0x000f) | ((instWord >> 3) & 0x0030) )<< 2);
  Uimm = ( ((instWord & 0x1c000000) >> 7) |((instWord >> 1) & 0x0010) | ((instWord  & 0x0020) ? 0xc0 : 0x0)) ; // for load and store
nzimm = ((instWord >> 2) & 0x001F) | (((instWord >> 12) & 1) ? 0xFFFFFFFE0 : 0); //immediate for ADDI set and for lui with ( << 12 )
imm = ((instWord >> 2) & 0x001F) | (((instWord >> 12) & 1) ? 0xFFFFFFFE0 : 0); // immediate can be zero for ANDI , LI
nzuimm = ((instWord >> 2) & 0x001F); //immediate  for srli,srai,slli
//Andimm = ((instWord >> 2) & 0x001F) | (((instWord >> 12) & 1) ? 0xFFFFFFE0 : 0);
nzimm_addi16sp = ((instWord >> 4) & 0x4) | ((instWord >> 2) & 0x8) | ((instWord >> 1) & 0x1c0) | ((instWord >> 7) & 0x30) | ((instWord >> 10 & 0x1) ? 0xFFFFFF80 : 0); //addi4spn
  Bimm = ((instWord >> 3) & 0x0003) | ((instWord >> 8) & 0x000C)|((instWord << 2) & 0x0010)|(instWord & 0x0060)|(((instWord >> 5) & 0x0080) ? 0xFFFFFF80 : 0);
 // Jimm = (instWord >> 2) & 0xE | (instWord >> 7) & 0x10 | (instWord << 3) & 0x20 | (instWord >> 1) & 0x340 | (instWord << 1 ) & 0x80 | (instWord << 2) & 0x400 | ((instWord >> 12 ) ? 0xFFFFF800 : 0);
  Jimm = ((instWord << 3) & 0x20) | ((instWord >> 2) & 0xE) | ((instWord << 1) & 0x80) | ((instWord >> 1) & 0x40) | ((instWord << 2) & 0x400) | ((instWord >> 1) & 0x300) | ((instWord >> 7) & 0x10) | (((instWord >> 12) & 0x1) ? 0xFFFFF800 : 0x0);
 
  opcode = instWord & 0x3;
  funct3 = ((instWord >> 13) & 0x0007);
  unsigned int temp = ((instWord >> 10) & 0x3); ;
  if (opcode == 0x0)
  {
   switch (funct3)
   {

       
//adds a zero-extended non-zero immediate, scaled by 4, to the stack pointer, x2, and writes the result to rd′.  expands to addi rd′, x2, nzuimm[9:2]. C.ADDI4SPN is only valid when nzuimm̸=0; 


//    case 0: 
//     rd = ((instWord >> 2) & 0x0007);
//     if (add14imm != 0)
//     {
//      cout << "  ADDI4SPN  x" << dec << rd << ", " << "(x2)" << ", " << hex << "0x" << (int)add14imm << endl;
//      regs[rd] = regs[2] + (int)add14imm * 4;
//     }
//     break;
   case 2: //load word
    //regs[rd] = ((memory[regs[rs1] + (int)I_imm])) & 0x000000ff;                     
    rd = ((instWord >> 2) & 0x0007);
    rs1 = ((instWord >> 7) & 0x0007);
    cout << "  C.LW x" << dec << rd << ", " << dec << (int)Uimm << "(x" << dec << rs1 << ")" << endl;
    
    regs[rd] = ((memory[regs[rs1] + (int)Uimm]));     // like how it was done in the memory
    break;

   
   case 6: // store word
    rs2 = ((instWord >> 2) & 0x0007);
    rs1 = ((instWord >> 7) & 0x0007);
    cout << "  C.SW  x" <<  dec <<rs2 << ", " << dec << (int)Uimm << "(x" << dec <<rs1 << ")" << endl;
    memory[regs[rs1] + Uimm] = regs[rs2];
    break;
   }

  }
  else if (opcode == 0x1)
  {
   
   switch (funct3)
   {
//C.ADDI adds the non-zero sign-extended 6-bit immediate to the value in register rd then writes the result to rd. C.ADDI expands into addi rd, rd, nzimm[5:0].
    case 0:
    rd = ((instWord >> 7) & 0x001F);
    if (rd != 0)
    {
     cout << "\tC.ADDI\tx" << dec << rd << ", x" << rd << ", " << hex << "0x" << (int)nzimm << "\n";
     regs[rd] = regs[rd] + (int)nzimm;
    }
    else
//C.NOP is a CI-format instruction that does not change any user-visible state, except for advancing the pc and incrementing any applicable performance counters. 
    {
     cout << "\tC.NOP\tx" << "\n";
     regs[0] += 0;
    }
    break;

//unconditional control transfer, writes the address of the instruction following the jump (pc+2) to the link register, x1. C.JAL expands to jal x1, offset[11:1]
   case 1:
    rd = ((instWord >> 7) & 0x001F);
    cout << "\tC.JAL\t x1"  <<" , " <<hex << "0x" << (int)Jimm<<"\n";
    regs[1] = pc ;  
      pc += (int)Jimm - 2;         
    break;
//C.LI loads the sign-extended 6-bit immediate, imm, into register rd. C.LI expands into addi rd, x0, imm[5:0]. C.LI is only valid when rd̸=x0
   case 2:
    rd = ((instWord >> 7) & 0x001F);
    cout << "\tC.LI\tx" << dec << rd << " , " << hex << "0x" << (int)imm<<"\n";
   // if (rd != 0)
     regs[rd] = imm;
    break;
/*C.LUI loads the non - zero 6 - bit immediate field into bits 17–12 of the destination register, clears the bottom 12 bits, and sign -
extends bit 17 into all higher bits of the destination.C.LUI expands into lui rd, nzimm[17:12].*/
   case 3:
    
    rd = ((instWord >> 7) & 0x001F);
    if (rd != 0 || rd != 2)
    {
       unsigned int immli = (nzimm << 12);
     cout << "\tC.LUI\tx" <<dec<< rd << " , " << hex << "0x" << (int)immli;
     regs[rd] = (int)immli;
    regs[rd] = regs[rd] | ((regs[rd] >> 17 & 0x1) ? 0xFFFF0000: 0 ); //and sign extends bit 17 into all higher bits of the destination    ask about this 
    }
    else if (rd==2)
//ADDI16SP adds the non-zero sign-extended 6-bit immediate to the value in the stack pointer (sp=x2), where the immediate is scaled to represent multiples of 16 in the range (-512,496). 
// It expands into addi x2, x2, nzimm[9:4]   
    {
     cout << "\tC.ADDI16SP\tx" << dec<< rd << " , " << hex << "0x" << (int)nzimm_addi16sp;
     regs[2] = regs[2]+(int)nzimm_addi16sp;
    }
    break;

   case 4:
    rd = ((instWord >> 7) & 0x0007);
    switch (temp)
    {
    case 0:
     cout << "\tC.SRLI\tx" << dec << rd << ", x" << dec << rd << hex<< "0x" << (int)nzuimm << "\n";
      if (nzuimm != 0)
     regs[rd] = (unsigned int)regs[rd] >> (unsigned)nzuimm;
     else
     cout << "not valied because the immediate == 0";
     break;
    case 1:
     cout << "\tC.SRAI\tx" << dec << rd << ", x" <<  dec <<rd << hex << "0x" << (int)nzuimm << "\n";
     if (nzuimm != 0)
     regs[rd] = regs[rd] >> (unsigned)nzuimm;
     else
     cout << "not valied because the immediate == 0";
     break;
    case 2:
     cout << "\tC.ANDI\tx" << dec << rd << ", x" << dec << rd << ", " << hex << "0x" << (int)imm << "\n";
     regs[rd] = (unsigned)regs[rd] & (unsigned)imm;
     break;
    case 3:
     rs2= ((instWord >> 2) & 0x0007);
     unsigned temp2= ((instWord >> 5) & 0x0003);
     switch (temp2) 
     {
//C.SUB subtracts the value in register rs2′ from the value in register rd′, then writes the result to register rd′. C.SUB expands into sub rd′, rd′, rs2′
     case 0:
      cout << "\tC.SUB\tx" << dec << rd << ", x" << dec << rd << ", x" << rs2 << "\n";
      regs[rd] = regs[rd] - regs[rs2];
      break;
// XOR of the values in registers rd′ and rs2′, then writes the result to register rd′. C.XOR expands into xor rd′, rd′, rs2′.
     case 1:
      cout << "\tC.XOR\tx" << dec << rd << ", x" << dec << rd << ", x" << rs2 << "\n";
      regs[rd] = regs[rd] ^ regs[rs2];
      break;
// OR of the values in registers rd′ and rs2′, then writes the result to register rd′. C.OR expands into or rd′, rd′, rs2′.
     case 2:
      cout << "\tC.OR\tx" << dec << rd << ", x" << dec << rd << ", x" << rs2 << "\n";
      regs[rd] = regs[rd] | regs[rs2];
      break;

//AND of the values in registers rd′ and rs2′, then writes the result to register rd′. C.AND expands into and rd′, rd′, rs2′.
     case 3:
      cout << "\tC.AND\tx" << dec << rd << ", x" << dec << rd << ", x" << rs2 << "\n";
      regs[rd] = regs[rd] & regs[rs2];
      break;
     }
     break;
    }
    break;
//C.J performs an unconditional control transfer. The offset is sign-extended and added to the pc to form the jump target address. C.J expands to jal x0, offset[11:1].
   case 5:
    cout << "\tC.J\tx0\t" << hex << "0x" << (int)Jimm << "\n"; 
    regs[0]= 0 ;
    pc = (pc-2) + (int)Jimm;
    break;
//C.BEQZ takes the branch if the value in register rs1′ is zero. It expands to beq rs1′, x0, offset[8:1].
   case 6:
    rs1= ((instWord >> 7) & 0x0007);
    cout << "\tC.BEQZ\tx" << dec << rs1 << ", x0"  << hex << "0x" << (int)Bimm; 
    if (regs[rs1] == 0)
     pc = pc-2 + (int)Bimm ;
    break;
//it takes the branch if rs1′ contains a nonzero value. It expands to bne rs1′, x0, offset[8:1].
   case 7:
    rs1 = ((instWord >> 7) & 0x0007);
    cout << "\tC.BNEZ\tx" << dec <<  rs1 << ", x0" << hex << "0x" << (int)Bimm;
    if (regs[rs1] != 0)
     pc = pc-2 + (int)Bimm ;
    break;
   }
   

  }
  else if (opcode==0x2)
  {
       unsigned int quad2temp= ((instWord >> 12) & 0x0001);
   switch (funct3)
   {
//C.SLLI is a CI-format instruction that performs a logical left shift of the value in register rd then writes the result to rd.
   case 0:
    rd = ((instWord >> 7) & 0x001F);
    cout << "\tC.SLLI\tx" << dec << rd << ", x" << dec << rd << ", " << hex << "0x" << (int)nzimm << "\n";
    regs[rd] = regs[rd] << (int)nzimm;
    break;
   case 4:
    rd = ((instWord >> 7) & 0x001F);
   
    rs2 = ((instWord >> 2) & 0x001F);
    switch (quad2temp)
    {
//copies the value in register rs2 into register rd. C.MV expands into add rd, x0, rs2
    case 0:
     //C.JR (jump register) performs an unconditional control transfer to the address in register rs1. C.JR expands to jalr x0, 0(rs1). 
  
  if (rs2 == 0)
  {
    rs1 = ((instWord >> 7) & 0x001F);
   cout << "\tC.JR\tx" <<dec<< rs1 << "\n";
   regs[0]= pc;
   pc = regs[rs1];
  }
//copies the value in register rs2 into register rd. C.MV expands into add rd, x0, rs2
  else
  {
   cout << "\tC.MV\tx" << dec << rd << ", x" << dec << rs2 << "\n";
   regs[rd] = regs[rs2];
  }
     break;
//C.ADD adds the values in registers rd and rs2 and writes the result to register rd. C.ADD expands into add rd, rd, rs2. C.ADD is only valid when rs2̸=x0; 
    case 1:
     if (rs2 != 0)
     {
      cout << "\tC.ADD\tx" << dec << rd << ", x" << dec << rd << ", x" << dec << rs2 <<"\n";
      regs[rd] = regs[rd] + regs[rs2];
     }
//JALR (jump and link register) performs the same operation as C.JR, but additionally writes the address of the instruction following the jump (pc+2) to the link register, x1. C.JALR expands to jalr x1, 0(rs1). 
     else
     {
        rs1 = ((instWord >> 7) & 0x001F);
	    cout << "\tC.JALR\tx1\t "  << dec << rs1 << "\n";
    	regs[1] = pc ;
        if (rs1 != 0)
    	pc = regs[rs1];
        else
        cout << " not valid because rs1 == x0" << endl;
     }
     break;
    }
    break;
    //loads a 32-bit value from memory into register rd. It computes an effective address by adding the zero-extended offset, scaled by 4, to the stack pointer, x2.
//It expands to lw rd, offset[7:2](x2). 
   case 2:
    rd = ((instWord >> 7) & 0x001F);
    cout << "\tC.LWSP\tx" << dec << rd << ", " << hex << "0x" << (int)lwspimm << ", (x2)" << "\n";
    regs[rd] = ((memory[regs[2] + (int)lwspimm*4]));
    break;


//stores a 32bit value in register rs2 to memory.It computes an effective address by adding the zero - extended offset, scaled by 4, to the stack pointer, x2.
//It expands to sw rs2, offset[7:2](x2).
   case 6:
    rs2 = ((instWord >> 2) & 0x001F);
    cout << "\tC.SWSP\tx" << dec<< rs2 << ", " << hex << "0x" << (int)swspimm <<  ", (x2)" << "\n";
    (memory[regs[2] + (int)swspimm * 4]) = regs[rs2];
    break;

   }
  }
 }

}

int main(int argc, char* argv[]) 
{
	unsigned int instWord = 0;
	ifstream inFile;
	ofstream outFile;
	unsigned int firstbytes = 0;

	if (argc < 1) emitError((char*)"use: rv32i_sim <machine_code_file_name>\n");
	inFile.open(argv[1], ios::in | ios::binary | ios::ate);
	if (inFile.is_open())
	{
		int fsize = inFile.tellg();
		inFile.seekg(0, inFile.beg);
		if (!inFile.read((char*)memory, fsize)) emitError((char*)"Cannot read from input file\n");
		else
		{
			while (flag)
			{
				instWord = (unsigned char)memory[pc] |
					(((unsigned char)memory[pc + 1]) << 8);
				firstbytes = instWord & 0x0003;
				if (firstbytes == 0x3)
				{
					instWord = instWord | (((unsigned char)memory[pc + 2]) << 16) |
						(((unsigned char)memory[pc + 3]) << 24);
                        compressed = false;
                        	pc = pc + 4;
					instDecExec(instWord);
				

				}
				else
				{
                    compressed = true;
                    pc = pc + 2;
					instDecExec(instWord);
					
					

				}

			}
		// 	while (flag) {
		// 	 instWord = (unsigned char)memory[pc] |
		// 	  (((unsigned char)memory[pc + 1]) << 8) |
		// 	  (((unsigned char)memory[pc + 2]) << 16) |
		// 	  (((unsigned char)memory[pc + 3]) << 24);
		// 	 pc += 4;
		// 	  //remove the following line once you have a complete simulator
		// 	 //if (pc == 32) break;   // stop when PC reached address 32
		// 	 instDecExec(instWord);
		// 	}
		// 	// dump the registers
		// 	for (int i = 0; i < 32; i++)
		// 		cout << "x" << dec << i << ": \t" << "0x" << hex << std::setfill('0') << std::setw(8) << regs[i] << "\n";
		 }
	}
	else
		emitError((char*)"Cannot access input file\n");
}

