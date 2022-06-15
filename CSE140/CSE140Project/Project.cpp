/*
Made by:
Andre Martin
Jessie Herrera 
University of California Merced
CSE 140 Project 
Spring Semester 2022
*/
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;
/*
    REQUIREMENT FUCNTIONS:
    Part 1(Single-cycle MIPS CPU):
        MAIN():
            Open and read text file
            CALL FETCH()

        FETCH():
            GLOBAL VAR: pc = 0
            reads one instruction from input
            PC: initialize as 0, then increment by 4 per instruction. (4*i)
            NEXT PC: declare as next_pc, stores pc + 4
            fucntion copies one of three possible pc values: next_pc, branch_target, or jump_target, to PC variable

        Decode():
            similar to HW3 but reads values from registers
            GLOBAL VAR: jump_target = 0
            Register File: int registerfile[32] //initialize to all zeros unless specified
                register file ID ($zero) = registerfile [0]
            Sign-extension: for offset field of I-type instructions
            Jump target address: calculate jump instruction target address. To update take last 26 bits from instruction, run shift-left-2, then merge with first 4 bits of next_pc variable.

        Execute():
            uses ControlUnit() signals
            executes computations of ALU, using register values and sign-extended offset from Decode().
            GLOBAL VAR:alu-zero = 0, branch_target = 0
            ALU OP:4-bit alu_op input
            Zero output: 1-bit zero output (alu-zero). updated from Execute() and decides PC value of next cycle. set from ALU when computation result is zero.
            Branch target address: calculate branch target address. updated by Execute() and used by Fetch(), decides PC value of next cycles. To update, shift-left-2 of sign extended offset input, then add shift-left-2 output with PC+4 value

        MEM():
            uses ControlUnit() signals
            Data memory: int d-mem[32]; initialize with all zeros unless otherwise specified
            each entry = 1 4-byte memory space
            begins from 0x0 (d-mem[0]), so each entry will be, (0x0 = d-mem[0], 0x4 = d-mem[1], 0x8 = d-mem[2], ...)
            receive memory address for both LW and SW and receive data to write to the memory. LW should be retrieved and sent to Writeback() function.

        Writeback():
            uses ControlUnit() signals
            GLOBAL VAR: total_clock_cycles = 0
            gets computation results from ALU and a data from Data memory, update destination register in registerfile array(Decode()).
            last step of instruction execution so cycle count increments +1.

        ControlUnit():
            GLOBAL VAR: RegWrite, RegDst, and so on = 0
            In Decode(), calls ControlUnit() to generate control signals.
            Receives 6-bit opcode values and generates 9 control signals (RegWrite, RegDst, and so on)
            ALU Control:recieves input fron ControlUnit and generates proper ALU OP code that is used by Execute()
                can create separate function or just merge with ControlUnit()
        
    Part 2(Extended MIPS CPU):
            Support JAL and JR
            Adds new control signals for these three instructions
            JAL:
                1)jump to target instruction
                2)update $ra register with PC+4 value
            JR:
                1)read $ra register value
                2)jump to address contained in $ra
*/
//Declare and initialize global variables
	int rd = 0;
	int rs = 0;
	int rt = 0;
	int address;
	int shamt;
//Fetch()
    int pc = 0;
//Decode()
    int registerfile[31] = {0}; // initialize registerfile entries with 0
    int jump_target = 0;
//Execute()
    int alu_zero = 0;
    int branch_target = 0;
//Mem()
    int d_mem [31] = {0}; // initialize d_mem entries with 0
//Writeback()
    int total_clock_cycles = 0;   
//ControlUnit()
    int branch = 0;
    int aluSRC = 0;
    int memToReg = 0;
    int regWrite = 0;
    int regDST = 0;
    int jump = 0;
    int jumpREG = 0;
    int memRead = 0;
    int memWrite = 0;

//ControlUnit Function
//GLOBAL VAR: RegWrite, RegDst, and so on = 0
//In Decode(), calls ControlUnit() to generate control signals.
//Receives 6-bit opcode values and generates 9 control signals (RegWrite, RegDst, and so on)
//ALU Control:recieves input fron ControlUnit and generates proper ALU OP code that is used by Execute()
//can create separate function or just merge with ControlUnit()
void ControlUnit(string op){
    // ADD, SUB, AND, OR, NOR, SLT
    if(op == "000000"){
        branch = 0;
        aluSRC = 0;
        memWrite = 0;
        memToReg = 0;
        memRead = 0;
        jump = 0;
        regWrite = 1;
        regDST = 1;
    }
    //LW
    else if(op == "100011"){
        branch = 0;
        aluSRC = 1;
        memWrite = 0;
        memToReg = 1;
        memRead = 1;
        jump = 0;
        regWrite = 1;
        regDST = 0;
    }
    //SW
    else if(op == "101011"){
        branch = 0;
        aluSRC = 1;
        memWrite = 1;
        memRead = 0;
        jump = 0;
        regWrite = 0;
    }
    //BEQ
    else if(op == "000100"){
        branch = 1;
        aluSRC = 0;
        memWrite = 0;
        memRead = 0;
        jump = 0;
        regWrite = 0;
    }
    //J format
    else if(op == "000010"){
        branch = 0;
        memWrite = 0;
        memRead = 0;
        jump = 1;
        regWrite = 0;
    }
    //JAL
    else if(op == "000011"){
        regDST = 10;
        memWrite = 0;
        memRead = 0;
        memToReg = 10;
        jump = 1;
        regWrite = 1;
    }
    //JR (op == 001000)
    else{
        branch = 0;
        memWrite = 0;
        jumpREG = 1;
        jump = 1;
        regWrite = 0;
    }
}

string aluControl(string code){
    //Separate the function from the control unit.
	string funct;
    if (code == "100100"){ //AND
        funct = "0000";
    }
    if (code == "100101"){ //OR
        funct = "0001";
    }
    if (code == "100000" || code == "101011" || code == "100011"){ // SW and LW ADD                                                              
        funct = "0010";
    }
    if (code == "100010" || code == "000100"){ //SUB and BEQ
        funct = "0110";
    }
    if (code == "101010"){ //SLT
        funct = "0111";
    }
    if (code == "100111"){ //NOR
        funct = "1100";
    }
    return funct;
}

string negCheck(string immediate){
    for (int i = 0; i < immediate.size(); i++){
        if (immediate[i] == '1'){
            immediate[i] = '0';}
        else{
            immediate[i] = '1';}
    };
    for (int i = immediate.size() - 1; i > 0; i--){
        if (immediate[i] == '1'){
            immediate[i] = '0';}
        else{
            immediate[i] = '1';
            break;}
    }
    return immediate;
}
int immCheck(string immediate){
	if (immediate[0] == '1'){
        immediate = "-" + negCheck(immediate);}
    return stoi(immediate, nullptr, 2);
}
int binToDec(string address){
    return stoi(address, nullptr, 2);
}

void regVal(int reg){
    if(reg == 0){cout << "zero";}
    if(reg == 1){cout << "at";}
    if(reg == 2){cout << "v0";}
    if(reg == 3){cout << "v1";}
    if(reg == 4){cout << "a0";}
    if(reg == 5){cout << "a1";}
    if(reg == 6){cout << "a2";}
    if(reg == 7){cout << "a3";}
    if(reg == 8){cout << "t0";}
    if(reg == 9){cout << "t1";}
    if(reg == 10){cout << "t2";}
    if(reg == 11){cout << "t3";}
    if(reg == 12){cout << "t4";}
    if(reg == 13){cout << "t5";}
    if(reg == 14){cout << "t6";}
    if(reg == 15){cout << "t7";}
    if(reg == 16){cout << "s0";}
    if(reg == 17){cout << "s1";}
    if(reg == 18){cout << "s2";}
    if(reg == 19){cout << "s3";}
    if(reg == 20){cout << "s4";}
    if(reg == 21){cout << "s5";}
    if(reg == 22){cout << "s6";}
    if(reg == 23){cout << "s7";}
    if(reg == 24){cout << "t8";}
    if(reg == 25){cout << "t9";}
    if(reg == 26){cout << "k0";}
    if(reg == 27){cout << "k1";}
    if(reg == 28){cout << "gp";}
    if(reg == 29){cout << "sp";}
    if(reg == 30){cout << "fp";}
    if(reg == 31){cout << "ra";}
}

//Writeback function
//uses ControlUnit() signals
//GLOBAL VAR: total_clock_cycles = 0
//gets computation results from ALU and a data from Data memory, update destination register in registerfile array(Decode()).
//last step of instruction execution so cycle count increments +1.
void Writeback(int reg, int result){
    if (regWrite == 0 && memWrite == 1){ // SW
        d_mem[reg] = result;
        cout << "memory 0x" << hex << reg << " is modified to 0x" << hex << result << endl;
    }else if (regWrite == 1){ // R AND LW
        registerfile[reg] = result;
        cout << "$";
        regVal(reg);
        cout << " is modified to 0x" << hex << result << endl;
    }
    total_clock_cycles = total_clock_cycles + 1;
}

//Mem function
//uses ControlUnit() signals
//Data memory: int d-mem[32]; initialize with all zeros unless otherwise specified
//each entry = 1 4-byte memory space
//begins from 0x0 (d-mem[0]), so each entry will be, (0x0 = d-mem[0], 0x4 = d-mem[1], 0x8 = d-mem[2], ...)
//receive memory address for both LW and SW and receive data to write to the memory. LW should be retrieved and sent to Writeback() function.
void Mem(int reg, int address){
    int val = 0;
    if (memWrite == 1){
        val = registerfile[reg];
        Writeback(address, val);
    }
    else if (memRead == 1){
        val = d_mem[address/4];
        Writeback(reg, val);
    }
    else{
        Writeback(reg, address);
    }
}

//Execute function
//uses ControlUnit() signals
//executes computations of ALU, using register values and sign-extended offset from Decode().
//GLOBAL VAR:alu-zero = 0, branch_target = 0
//ALU OP:4-bit alu_op input
//Zero output: 1-bit zero output (alu-zero). updated from Execute() and decides PC value of next cycle. set from ALU when computation result is zero.
//Branch target address: calculate branch target address. updated by Execute() and used by Fetch(), decides PC value of next cycles. To update, shift-left-2 of sign extended offset input, then add shift-left-2 output with PC+4 value
void Execute(string alu_op, int rs, int rt, int rd, int shamt, int address){
    int val = 0;
    if(alu_op == "0000"){
        val = registerfile[rs] & registerfile[rt]; 
    } 
    if(alu_op == "0001"){
        val = registerfile[rs] | registerfile[rt]; 
    }
    if(alu_op == "0110"){
        val = registerfile[rs] - registerfile[rt]; 
    }
    if(alu_op == "1100"){
        val = ~(registerfile[rs] | registerfile[rt]); 
    }
    if(alu_op == "0010"){
        if(aluSRC == 1){
            val = registerfile[rs] + address;
        }
        else{
            val = registerfile[rs] + registerfile[rt];
        }
    }
    if(alu_op == "0111"){
        if(registerfile[rs] < registerfile[rt]){
            val = 1;
        }
        else{
            val = 0;
        }
    }
    if(memToReg == 10){
        Mem(jump_target, address);
    }
    if(val == 0 && branch == 1){
        alu_zero = 1;
    }
    else{
        alu_zero = 0;
    }
    if(memWrite == 1 || memRead == 1){
        Mem(rt, val);
    }
    else{
        Mem(rd, val);
    }
}

//Decode function
//similar to HW3 but reads values from registers
//Register File: int registerfile[32] //initialize to all zeros unless specified
    //register file ID ($zero) = registerfile [0]
//Sign-extension: for offset field of I-type instructions
//Jump target address: calculate jump instruction target address. To update take last 26 bits from instruction, run shift-left-2, then merge with first 4 bits of next_pc variable.
void Decode(string instruction){
	string function;
	string immediate;
	string alu_op;
	string sixbits = instruction.substr(0, 6);
	ControlUnit(sixbits);
	if(jump == 1){
		immediate = instruction.substr(6, 26);
		address = immCheck(immediate);
		address = address << 2;
		jump_target = address;
	}
    else if(branch == 1){
		rs = binToDec(instruction.substr(6, 5));
        rt = binToDec(instruction.substr(11, 5));
		alu_op = aluControl(sixbits);
		immediate = instruction.substr(16,16);
		address = immCheck(immediate);
		address = address << 2;
		branch_target = address;
	}
    else{
		rs = binToDec(instruction.substr(6, 5));
        if(regDST == 1 && regWrite == 1){
            rd = binToDec(instruction.substr(16, 5));
        }
        else{ //LW and SW
            rt = binToDec(instruction.substr(11, 5));
        }
        if(aluSRC == 0){ // R
            rt = binToDec(instruction.substr(11, 5));
            shamt = binToDec(instruction.substr(21,5));
            function = instruction.substr(26, 6);
            alu_op = aluControl(function); //alu_op determined by funct  
        }
        else{ // Immediate LW and SW
            immediate = instruction.substr(16, 16);
            alu_op = aluControl(sixbits); //alu_op determined by opcode
            address = immCheck(immediate);
        }
	}
    Execute(alu_op, rs, rt, rd, shamt, address);
}

//Fetch function
//reads one instruction from input
//PC: initialize as 0, then increment by 4 per instruction. (4*i)
//NEXT PC: declare as next_pc, stores pc + 4
//fucntion copies one of three possible pc values: next_pc, branch_target, or jump_target, to PC variable
void Fetch(vector<string> instruction){
	while(pc/4 < instruction.size()){
		cout << "total_clock_cycles " << dec << total_clock_cycles + 1 << " :" << endl;
        Decode(instruction[pc/4]);
	    int next_pc = pc + 4;
		//copy one of three possibilites to pc variable
		if (alu_zero == 1 && branch == 1){
            pc = next_pc + branch_target;
        }
        else if(jump == 1){
            pc = jump_target;
        }
        else{
            pc = next_pc;
        }
		cout << "pc is modified to 0x" << hex << pc << endl << endl;
	}
	cout << "program terminated" << endl;
	cout << "total execution time is " << total_clock_cycles << " cycles" << endl;
}

int main(){
    //copy instruction from file
	vector<string> machineCode;
	string instruction;
	string file;
    //user input
    cout << "Enter file name to run:\n";
    cin >> file;

    //part 1 of project
    registerfile[9] = 0x20;
    registerfile[10] = 0x5;
    registerfile[16] = 0x70;
    d_mem[28] = 0x5;
    d_mem[29] = 0x10;
    
   
    //part 2 of project
    /*
    registerfile[16] = 0x20;
    registerfile[4] = 0x5;
    registerfile[5] = 0x2;
    registerfile[7] = 0xa;
    */

    ifstream filename(file);
    while(getline(filename, instruction)){
        machineCode.push_back(instruction);
    }
    Fetch(machineCode);
}
