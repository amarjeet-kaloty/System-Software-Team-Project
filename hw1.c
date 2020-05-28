#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_STACK_HEIGHT  1000
#define MAX_CODE_LENGTH  500

typedef struct instruction{
  int op; // opcode
  int l; // lexicographical level
  int m; // M(Value).
}instruction;

//functions declarations
char* getOpcode(instruction x);

instruction code[MAX_CODE_LENGTH];
int stack[MAX_DATA_STACK_HEIGHT] = {0};
instruction ir;
int sp = MAX_DATA_STACK_HEIGHT; //stack pointer
int bp = 999;  //base pointer
int pc = 0; //program counter

int main()
{
    int i = 0, x = 0;

	FILE *fp = fopen("input.txt", "r");

	if(fp == NULL){
		printf("ERROR OPENING FILE!!\n");
	}
    
    //read the first line from input file
	fscanf(fp, "%d%d%d", &code[i].op, &code[i].l, &code[i].m);

	//read in all instructions from the input file
	while(!feof(fp)){
		i++;
		fscanf(fp, "%d%d%d", &code[i].op, &code[i].l, &code[i].m);
	}

	fclose(fp);

	fp = fopen("output.txt", "w");

	if(fp == NULL){
		printf("ERROR OPENING FILE!!\n");
	}

	//format and output line headers
	fprintf(fp, "%-5s  %-5s%-5s%-5s\n", "Line", "OP", "L", "M");

	//print out our correctly formatted instructions
	for(x = 0; x < i; x++){
		char op[4];
		strcpy(op, getOpcode(code[x]));
		fprintf(fp, "%4d  %-5s %-5d%-5d\n", x, op, code[x].l, code[x].m);
	}
	fprintf(fp, "\n\n");
    return 0;
}

char* getOpcode(instruction x){
	switch(x.op){
		case 1:
		return "lit";
		//OPR
		case 2:
		switch(x.m){
			case 0:
			    if(sp==34){
						return "sio";
				}
			return "opr";
			case 1:
			return "neg";
			case 2:
			return "add";
			case 3:
			return "sub";
			case 4:
			return "mul";
			case 5:
			return "div";
			case 6:
			return "odd";
			case 7:
			return "mod";
			case 8:
			return "eql";
			case 9:
			return "neq";
			case 10:
			return "lss";
			case 11:
			return "leq";
			case 12:
			return "gtr";
			case 13:
			return "geq";
		}

		case 3:
		return "lod";
		case 4:
		return "sto";
		case 5:
		return "cal";
		case 6:
		return "inc";
		case 7:
		return "jmp";
		case 8:
		return "jpc";
		case 9:
		return "sio";
		case 10:
		return "sio";
		case 11:
        return "sio";
		default: return "";
	}
}

