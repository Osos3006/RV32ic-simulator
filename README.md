# RV32ic-simulator

 All copy rights © goes to Mohammed Osama Abuelwafa and Rinal Hamdy 
 
 This c++ program works as a disassembler and simulator for Risc-v instruction set architecture. it supports RV32IC instructions ( integer and compressed instructions).
 The simulator is able to do the following:
 
 1) Read RV32IC machine code file. The file is just a binary file that contains the machine code of a program
instructions. The first 4 bytes (32-bit word) in the file belong to the first instruction; the 2nd 4 bytes belong to
the 2nd instruction, etc. The first instruction is assumed to be at location 0x00000000 in the main memory.

2) Decode each machine code word, then translate it into a true RV32IC instruction.

3) Execute the decoded instruction. The execution involves: modifying a register, modifying a memory location. or performing an I/O operation (ECALL).

4) Print out the decoded instruction on the screen.

5)Be invoked from the command line using the syntax:
rvsim <machine_code_file_name>
• Where:
<machine_code_file_name> : is the file name of the input machine code
rvsim : is the name of the executable file of the cpp (i.e. it can be anything).

Error handelling is provided as following:

1) if an unsupported instruction is provided in the binary, the program prints " unknown instruction ".

2) validation is provided as well. for example: some instructions has some restrictions as for the compressed, some instructions requires rs2 to not be equal to zero. hence, if such a case occurs, the execution is not processed.


//HOW TO BUILD AND USE IT?
 
 1) put the source file in a folder 
 2) you must have a c compiler. gcc compiler was used in the development process and testing.
 
 write the following to have your executable:
 
 g++  "source name".cpp -o executable name
 
 <executable_name>       <machine_code_file_name>
 
 notes: machine code file name is used directly if it is within the same folder, otherwise its path must be included.
 
 3) Run Your Program !!


