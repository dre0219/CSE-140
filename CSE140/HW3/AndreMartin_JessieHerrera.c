#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
// The Functions
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
    printf("Enter an instruction in machine code:\n");
    //We create a variable in order to hold the instruction.
    char instruction[32]; 

    //This gets the instruction 
    scanf("%s", instruction);
    printf("\n");

    //We then call the function to convert instruction to MIPS
    DecodingInstruction(instruction);
}

void DecodingInstruction(char  instruction[]){
    //Getting the substring of the OPcode field
    //printf("instruction: %s\n", instruction); (side note)
    char OPcodeBin[7];
    memcpy(&OPcodeBin, &instruction[0], 6);
    OPcodeBin[6] = '\0';

    //We convert the OPcode to an integer 
    //printf("OPcode: %s\n", OPcodeBin); (side note)
    int OPcode = CoBTI(OPcodeBin);

    //We check the OPcode for its correspondence in instruction types
    if(OPcode == 0){
        //The function in calling R-type instructions
        DecodingTypeR(instruction);
    }else if(OPcode == 2 || OPcode == 3){
        //The function in calling J-type instructions
        DecodingTypeJ(instruction);
    }else{
        //The function in calling I-type instructions
        DecodingTypeI(instruction);
    }
}

void DecodingTypeI(char instruction[]){
    //Printing the instruction type
    printf("Instruction Type: I\n");

    //Grabbing the bits from the OPcode field 
    char OPcodeBin[7]; 
    memcpy(&OPcodeBin, &instruction[0], 6);
    OPcodeBin[6] = '\0';

    //Converting OPcode to decimal 
    int OPcode = CoBTI(OPcodeBin);
    
    //Getting the name of OPcode and print
    char *Function = OperationTypeI(OPcode);
    printf("Operation: %s\n", Function);

    //Gets the bits of rs 
    char rsBin[6]; 
    memcpy(&rsBin, &instruction[6], 5);
    rsBin[5] = '\0';

    //Converting the decimal to get name and print
    int rs = CoBTI(rsBin);
    char *rsName = Register(rs);
    printf("Rs: %s (R%d)\n", rsName, rs);

    //This grabs the bits relevant to rt 
    char rtBin[6]; 
    memcpy(&rtBin, &instruction[11], 5);
    rsBin[5] = '\0';

    //Converts the decimal and gets the name, then prints it 
    int rt = CoBTI(rtBin);
    char *rtName = Register(rt);
    printf("Rt: %s (R%d)\n", rtName, rt);

    char immBin[17]; 
    memcpy(&immBin, &instruction[16], 16);
    immBin[16] = '\0';
    
    /*We convert the decimal and to get the instruction name, use the two's compliment
    unless the OPcode is unsigned then we use the default*/
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
    printf("Instruction Type: J\n");
    //Gets the substring of the OPcode field
    //printf("instruction: %s\n", instruction); 
    char OPcodeBin[7];
    memcpy(&OPcodeBin, &instruction[0], 6);
    OPcodeBin[6] = '\0';

    //This converts the OPcode to an integer 
    //printf("OPcode: %s\n", OPcodeBin);
    int OPcode = CoBTI(OPcodeBin);

    //This prints the name of the function 
    if (OPcode == 2){
        char *Function = "j";
        printf("Operation: %s\n", Function);
    } else {
        char *Function = "jal";
        printf("Operation: %s\n", Function);
    }

    //This grabs bits of the address field. 
    char addressBin[27];
    memcpy(&addressBin, &instruction[6], 26);
    addressBin[26] = '\0';
    
    //Converts the address to a decimal
    int address = CoBTI(addressBin);
    printf("Address: %d\n", address);
}

void DecodingTypeR(char instruction[]){
    printf("Instruction Type: R\n");

    //This gets bits from the function field 
    char functionBin[7]; 
    memcpy(&functionBin, &instruction[26], 6);
    functionBin[6] = '\0';

    //Converts the function code to a decimal 
    int function = CoBTI(functionBin);

    //This names the corresponding function and prints.
    char *Function = FunctionTypeR(function);
    printf("Operation: %s\n", Function);
    
    if(function == 8){
        //The instruction jr only uses rs field
        //Grabs bits of rs 
        char rsBin[6]; 
        memcpy(&rsBin, &instruction[6], 5);
        rsBin[5] = '\0';
        //Converting the decimal to get a name and print
        int rs = CoBTI(rsBin);
        char *rsName = Register(rs);
        printf("Rs: %s (R%d)\n", rsName, rs);
        printf("Rt: NOT USED\n");  //Rt is not used
        printf("Rd: NOT USED\n"); //Rd is not used
        printf("Shamt: 0\n");  //Shamt is not being used so it is set to 0
    }else if(function == 0 || function == 2){
        //sll and srl use fields: Rd, Rt, and Shamt
        printf("Rs: NOT USED\n");  //Rs is not being used
        //This gets bits of Rt 
        char rtBin[6]; 
        memcpy(&rtBin, &instruction[11], 5);
        rtBin[5] = '\0';

        //Conversion of decimal to get the name and print
        int rt = CoBTI(rtBin);
        char *rtName = Register(rt);
        printf("Rt: %s (R%d)\n", rtName, rt);

        //Grabs bits of Rd 
        char rdBin[6]; 
        memcpy(&rdBin, &instruction[16], 5);
        rdBin[5] = '\0';
        //Conversion of decimal to get the name and print
        int rd = CoBTI(rdBin);
        char *rdName = Register(rd);
        printf("Rd: %s (R%d)\n", rdName, rd);
        //Grabs bits of Shamt
        char shamtBin[6]; 
        memcpy(&shamtBin, &instruction[21], 5);
        rdBin[5] = '\0';
        //Conversion of decimal to get the name and print
        int shamt = CoBTI(shamtBin);
        printf("Shamt: %d", shamt);
    }else{
        //The rest of the operations utilize all three registers, but not Shamt
        char rsBin[6]; 
        memcpy(&rsBin, &instruction[6], 5);
        rsBin[5] = '\0';
        //Converting to decimal and getting the name, then printing it 
        int rs = CoBTI(rsBin);
        char *rsName = Register(rs);
        printf("Rs: %s (R%d)\n", rsName, rs);
        char rtBin[6]; 
        memcpy(&rtBin, &instruction[11], 5);
        rtBin[5] = '\0';
        //Converting to decimal and getting the name, then printing it 
        int rt = CoBTI(rtBin);
        char *rtName = Register(rt);
        printf("Rt: %s (R%d)\n", rtName, rt);
        char rdBin[6]; 
        memcpy(&rdBin, &instruction[16], 5);
        rdBin[5] = '\0';
        //Converting to decimal and getting the name, then printing it 
        int rd = CoBTI(rdBin);
        char *rdName = Register(rd);
        printf("Rd: %s (R%d)\n", rdName, rd);
        printf("Shamt: 0\n"); //Shamt isn't being used so it is set to 0
    } 
    printf("Function: %d\n", function);
}
//A function in order to convert a binary string to an integer
int CoBTI(char binary[]){
    //This grabs the length of the binary string
    int length = strlen(binary);
    double decimal = 0;

    //Iterating
    for(int i = 0; i < length; i++){
        if(binary[length - i - 1] == '1'){
            decimal += pow(2.0, (double) i);
        }
    }
    //Having a type cast to integer and return
    return (int) decimal;
}

//A function to convert a binary string to a negative integer using two's complement 
int CoBTITC(char twosBinary []){
    //Variable in order to hold the converted decimal 
    int decimal = 0;
    //Since the given number is already a two's complement reverse the bits and add 1
    int length = strlen(twosBinary);
    for(int i = 0; i < length; i++){
        if(twosBinary[i] == '1'){
            //If the bit is 1 it is flipped to 0
            twosBinary[i] = '0';
        }else{
            //If the bit is 0 it is flipped to 1
            twosBinary[i] = '1';
        }
    }
    for(int i = length - 1; i >= 0; i--){
        if(twosBinary[i] == '0'){
            twosBinary[i] = '1';
            break;
        }else{
            twosBinary[i] = '0';
        }
    }
    //Conversion
    decimal = -1 * CoBTI(twosBinary);
    return decimal; 
}

//This is a function in order to return the name of the register as a string given the registers number
char* Register(int Number){
    char *name;
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
//This is a function in order to return the name of the r type instructions 
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
//This is a function in order to return the name of the r type instructions 
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
    }
    return Operation;
}