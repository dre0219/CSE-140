#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

// function declarations 
void DecodingInstruction(char instruction[]);
void DecodingTypeI(char instruction[]);
void DecodingTypeJ(char instruction[]);
void DecodingTypeR(char instruction[]);
int CoBTI(char binary[]); // Conversion of Bin To Int
int CoBTITC(char twosBinary []); //Conversion of Bin To Int Twos Complement
char* Register(int Number);
char* FunctionTypeR(int function);
char* OperationTypeI(int OPcode);


int main(){
   
    //ask for input
    printf("Enter an instruction in machine code:\n");

    //create a variable to hold the instruction
    char instruction[32]; 

    //get the instruction 
    scanf("%s", instruction);
    printf("\n");

    //call function to convert instruction to MIPS
    DecodingInstruction(instruction);

    //testing twos complement function
    // int test = CoBTITC(instruction);
    // printf("Twos complement: %d\n", test);
    // return 0;
}

void DecodingInstruction(char  instruction[]){

    //take the substring relevant to the OPcode field
    //printf("instruction: %s\n", instruction);
    char OPcodeBin[7];
    memcpy(&OPcodeBin, &instruction[0], 6);
    OPcodeBin[6] = '\0';

    //convert the OPcode to be an integer 
    //printf("OPcode: %s\n", OPcodeBin);
    int OPcode = CoBTI(OPcodeBin);

    //check OPcode to match instruction to its type
    if(OPcode == 0){
        //instruction is R type call function
        DecodingTypeR(instruction);
    }else if(OPcode == 2 || OPcode == 3){
        //instruction is J type call function
        DecodingTypeJ(instruction);
    }else{
        //instruction is I type call fuction
        DecodingTypeI(instruction);
    }
}

void DecodingTypeI(char instruction[]){

    //print the instruction type
    printf("Instruction Type: I\n");

    //grab relevant bits for OPcode field 
    char OPcodeBin[7]; 
    memcpy(&OPcodeBin, &instruction[0], 6);
    OPcodeBin[6] = '\0';

    //convert OPcode to decimal 
    int OPcode = CoBTI(OPcodeBin);
    


    //find the relevant name for the given OPcode and print it
    char *Function = OperationTypeI(OPcode);
    printf("Operation: %s\n", Function);

    //grab bits relevant to rs 
    char rsBin[6]; 
    memcpy(&rsBin, &instruction[6], 5);
    rsBin[5] = '\0';

    //convert to decimal and get name, print it 
    int rs = CoBTI(rsBin);
    char *rsName = Register(rs);
    printf("Rs: %s (R%d)\n", rsName, rs);

    //grab bits relevant to rt 
    char rtBin[6]; 
    memcpy(&rtBin, &instruction[11], 5);
    rsBin[5] = '\0';

    //convert to decimal and get name, print it 
    int rt = CoBTI(rtBin);
    char *rtName = Register(rt);
    printf("Rt: %s (R%d)\n", rtName, rt);

    //grab bits relevant to immediate

    char immBin[17]; 
    memcpy(&immBin, &instruction[16], 16);
    immBin[16] = '\0';

    //printf("Immediate binary: %s\n", immBin);
    
    //convert to decimal and get name, print it 
    //if OPcode says unsigned, use default otherwise use 2's comp
    int imm;
    if (OPcode == 9 || OPcode == 36 || OPcode == 37 || OPcode == 11){
        imm = CoBTI(immBin);
        printf("Immediate: %d\n", imm);


    } else {
        if (immBin[0] == '1'){
            imm = CoBTITC(immBin);
            printf("Immediate: %d\n", imm);

        } else {

            imm = CoBTI(immBin);
            printf("Immediate: %d\n", imm);

        }
    }
    
    





}
 
void DecodingTypeJ(char instruction[]){

    //print the instruction type
    printf("Instruction Type: J\n");

    //take the substring relevant to the OPcode field
    //printf("instruction: %s\n", instruction);
    char OPcodeBin[7];
    memcpy(&OPcodeBin, &instruction[0], 6);
    OPcodeBin[6] = '\0';

    //convert the OPcode to be an integer 
    //printf("OPcode: %s\n", OPcodeBin);
    int OPcode = CoBTI(OPcodeBin);

    //find name for the given function and print it
    
    if (OPcode == 2){
        char *Function = "j";
        printf("Operation: %s\n", Function);
    } else {
        char *Function = "jal";
        printf("Operation: %s\n", Function);
    }
    
    



    //grab relevant bits for address field. 
    char addressBin[27];
    memcpy(&addressBin, &instruction[6], 26);
    addressBin[26] = '\0';
    
    //convert address to decimal
    
    int address = CoBTI(addressBin);
    //print address
    printf("Address: %d\n", address);
}

void DecodingTypeR(char instruction[]){

    //print the instruction type
    printf("Instruction Type: R\n");

    //grab relevant bits for function field 
    char functionBin[7]; 
    memcpy(&functionBin, &instruction[26], 6);
    functionBin[6] = '\0';

    //convert function code to decimal 
    int function = CoBTI(functionBin);
    //printf("function Binary: %s\n",functionBin);
    //printf("function num: %d\n", function);

    //find the relevant name for the given function code and print it
    char *Function = FunctionTypeR(function);
    printf("Operation: %s\n", Function);
    
    //take 
    if(function == 8){

        //this function is jr
        //only utilizes rs

         //grab bits relevant to rs 
        char rsBin[6]; 
        memcpy(&rsBin, &instruction[6], 5);
        rsBin[5] = '\0';
        //convert to decimal and get name, print it 
        int rs = CoBTI(rsBin);
        char *rsName = Register(rs);
        printf("Rs: %s (R%d)\n", rsName, rs);

        //rt and rd are not used, print not used
        printf("Rt: NOT USED\n");
        printf("Rd: NOT USED\n");

        //this operation does not use shamt, print it as 0
        printf("Shamt: 0\n");
    }else if(function == 0 || function == 2){

        //these instructions (sll and srl) utilize rd, rt, and shamt

        //since rs is not used, print it is not used
        printf("Rs: NOT USED\n");

        //grab the relevant information for rt and rd

         //grab bits relevant to rt 
        char rtBin[6]; 
        memcpy(&rtBin, &instruction[11], 5);
        rtBin[5] = '\0';
        //convert to decimal and get name, print it
        int rt = CoBTI(rtBin);
        char *rtName = Register(rt);
        printf("Rt: %s (R%d)\n", rtName, rt);


        //grab bits relevant to rd 
        char rdBin[6]; 
        memcpy(&rdBin, &instruction[16], 5);
        rdBin[5] = '\0';
        //convert to decimal and get name, print it
        int rd = CoBTI(rdBin);
        char *rdName = Register(rd);
        printf("Rd: %s (R%d)\n", rdName, rd);

        //grab relevant bits for shamt
        char shamtBin[6]; 
        memcpy(&shamtBin, &instruction[21], 5);
        rdBin[5] = '\0';
        //convert the shamt to decimal and print
        int shamt = CoBTI(shamtBin);
        printf("Shamt: %d", shamt);

    }else{

        //the rest of the operations utilize all three registers, no shamt
        
        //grab bits relevant to rs 
        char rsBin[6]; 
        memcpy(&rsBin, &instruction[6], 5);
        rsBin[5] = '\0';
        //convert to decimal and get name, print it 
        int rs = CoBTI(rsBin);
        char *rsName = Register(rs);
        printf("Rs: %s (R%d)\n", rsName, rs);

        //grab bits relevant to rt 
        char rtBin[6]; 
        memcpy(&rtBin, &instruction[11], 5);
        rtBin[5] = '\0';
        //convert to decimal and get name, print it
        int rt = CoBTI(rtBin);
        char *rtName = Register(rt);
        printf("Rt: %s (R%d)\n", rtName, rt);


        //grab bits relevant to rd 
        char rdBin[6]; 
        memcpy(&rdBin, &instruction[16], 5);
        rdBin[5] = '\0';
        //convert to decimal and get name, print it
        int rd = CoBTI(rdBin);
        char *rdName = Register(rd);
        printf("Rd: %s (R%d)\n", rdName, rd);

        //since none of these operations use shamt, print it as 0
        printf("Shamt: 0\n");
    }

  

    //all instructions print out the function code found above 
    printf("Funct: %d\n", function);


}


//helper functions to perform various repetitive and mundane tasks

//a function to convert a binary string to an integer
int CoBTI(char binary[]){

    //grab the length of the binary string
    int length = strlen(binary);
    //printf("length: %d\n", length);
    //return value 
    double decimal = 0;

    //iterate over the string 
    for(int i = 0; i < length; i++){
        //if the char is 1, apply relevant power of 2 and add to result
        if(binary[length - i - 1] == '1'){
            decimal += pow(2.0, (double) i);
        }
        //printf("Iteration: %d, decimal: %f\n", i, decimal);
    }

    //type cast to integer and return
    return (int) decimal;

}

//a function to convert a binary string to an negative integer using two's complement 
int CoBTITC(char twosBinary []){

    //a variable to hold the converted decimal 
    int decimal = 0;

    //since the given number is already a two's complement reverse the bits and add 1

    //first reverse the bits 
    int length = strlen(twosBinary);

    //printf("length: %d\n", length);
    for(int i = 0; i < length; i++){

        if(twosBinary[i] == '1'){
            
            //if bit is 1 flip it to 0
            twosBinary[i] = '0';

        }else{

            //if bit is 0 then flip it to 1
            twosBinary[i] = '1';
        }
    }
    //printf("twos flipped: %s\n", twosBinary);

    //now add 1 to the reversed bits 
    for(int i = length - 1; i >= 0; i--){

        //starting from the back flip 1's to 0 until a 0 is reached and flip it to a 1
        if(twosBinary[i] == '0'){
            twosBinary[i] = '1';
            break;
        }else{
            twosBinary[i] = '0';
        }
    }
    //printf("Converted twos binary: %s\n", twosBinary);
    //now the twos complement is ready to be converted to decimal and multiplied by -1
    decimal = -1 * CoBTI(twosBinary);
    return decimal; 
}

//a function to return the name of the register as a string given the registers number
char* Register(int Number){
    char *name;

    //set name based on register number
    if(Number == 0){
        name = "zero";
    }else if(Number == 1){
        name = "at";
    }else if(Number == 2){
        name = "v0";
    }else if(Number == 3){
        name = "v1";
    }else if(Number == 4){
        name = "a0";
    }else if(Number == 5){
        name = "a1";
    }else if(Number == 6){
        name = "a2";
    }else if(Number == 7){
        name = "a3";
    }else if(Number == 8){
        name = "t0";
    }else if(Number == 9){
        name = "t1";
    }else if(Number == 10){
        name = "t2";
    }else if(Number == 11){
        name = "t3";
    }else if(Number == 12){
        name = "t4";
    }else if(Number == 13){
        name = "t5";
    }else if(Number == 14){
        name = "t6";
    }else if(Number == 15){
        name = "t7";
    }else if(Number == 16){
        name = "s0";
    }else if(Number == 17){
        name = "s1";
    }else if(Number == 18){
        name = "s2";
    }else if(Number == 19){
        name = "s3";
    }else if(Number == 20){
        name = "s4";
    }else if(Number == 21){
        name = "s5";
    }else if(Number == 22){
        name = "s6";
    }else if(Number == 23){
        name = "s7";
    }else if(Number == 24){
        name = "t8";
    }else if(Number == 25){
        name = "t9";
    }else if(Number == 26){
        name = "k0";
    }else if(Number == 27){
        name = "k1";
    }else if(Number == 28){
        name = "gp";
    }else if(Number == 29){
        name = "sp";
    }else if(Number == 30){
        name = "fp";
    }else if(Number == 31){
        name = "ra";
    }
    return name;
}

//a function to return the name of an r type instructions 
char* FunctionTypeR(int function){
    char *Function;

    switch(function){
        case 32:
            Function = "add";
            break;
        case 33:
            Function = "addu";
            break;
        case 36:
            Function = "and";
            break;
        case 8:
            Function = "jr";
            break;
        case 39:
            Function = "nor";
            break;
        case 37:
            Function = "or";
            break;
        case 42:
            Function = "slt";
            break;
        case 43:
            Function = "sltu";
            break;
        case 0:
            Function = "sll";
            break;
        case 2:
            Function = "srl";
            break;
        case 34:
            Function = "sub";
            break;
        case 35:
            Function = "subu";
            break;
        default :
            Function = "ERROR";
    }

    return Function;
}

//a function to return the name of an r type instructions 
char* OperationTypeI(int OPcode){
    char *Operation;

    switch(OPcode){
        case 8:
            Operation = "addi";
            break;
        case 9:
            Operation = "addiu";
            break;
        case 12:
            Operation = "andi";
            break;
        case 4:
            Operation = "beq";
            break;
        case 5:
            Operation = "bne";
            break;
        case 36:
            Operation = "lbu";
            break;
        case 37:
            Operation = "lhu";
            break;
        case 43:
            Operation = "sw";
            break;
        case 15:
            Operation = "lui";
            break;
        case 35:
            Operation = "lw";
            break;
        case 13:
            Operation = "ori";
            break;
        case 48:
            Operation = "ll";
            break;
        case 10:
            Operation = "slti";
            break;
        case 11:
            Operation = "sltiu";
            break;
        case 40:
            Operation = "sb";
            break;
        case 56:
            Operation = "sc";
            break;
        case 41:
            Operation = "sh";
            break;
        default :
            Operation = "ERROR";

        //need load link, SLTI, sltiu, sb, sc, sh
    }

    return Operation;
}