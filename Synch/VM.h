//   Student Name:
// Ashwin Jegatheesan
// Amarjeet S Kaloty
#ifndef VM.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_STACK_HEIGHT  1000
#define MAX_CODE_LENGTH  1000

typedef struct instruction{
  int op; // opcode
  int l; // lexicographical level
  int m; // M(Value).
}instruction;

//Function declarations
char* getOpcode(instruction x);
int base(int x, int base);
void printStack(int status, int flag);

//Variable Declarations
instruction code[MAX_CODE_LENGTH];
int stack[MAX_DATA_STACK_HEIGHT] = {0};
instruction ir;
int sp = MAX_DATA_STACK_HEIGHT; //stack pointer
int bp = 999;  //base pointer
int pc = 0; //program counter
int halt = 0;
int arr[2000] = {0};
int ars = 0;
int prev = 0;

int VM(int flag){
    int i = 0;

	FILE *fp = fopen("code.txt", "r");

	if(fp == NULL){
		printf("ERROR OPENING FILE!!\n");
	}

    //This line of code will read from the scan from the input file and place it in our instruction code
	fscanf(fp, "%d%d%d", &code[i].op, &code[i].l, &code[i].m);

	//read in all instructions from the input file
	while(!feof(fp)){
		i++;
		fscanf(fp, "%d%d%d", &code[i].op, &code[i].l, &code[i].m);
	}

	fclose(fp);

	//This will help us format our line , op, l, and m to look properly
	if(flag)
		printf( "%-5s  %-5s%-5s%-5s\n", "Line", "OP", "L", "M");

	//print out our correctly formatted instructions
	for(int x = 0; x < i; x++){
		char opcode[4];
		strcpy(opcode, getOpcode(code[x]));
    	if(flag)
			printf( "%4d  %-5s %-5d%-5d\n", x, opcode, code[x].l, code[x].m);
	}
	if(flag)
		printf("\n\n");


	if(flag){
		printf("                    %-5s%-5s%-6s%s\n", "pc", "bp", "sp", "stack");
		printf("Initial Values      %-5d%-5d%-5d\n", 0,999,1000);
	}

	while(!halt){

		//Fetch
		ir = code[pc];
    	prev=pc;  // instruction goes into the instruction register.
    	pc = pc+1; //incrementing the program counter

		//Print
		printStack(1, flag);

		//Execute
		switch(ir.op){
			//LIT
			case 1:
				sp = sp - 1;
				stack[sp] = ir.m;
			break;
			//OPR, so m must be evaluated.
			case 2:
				switch(ir.m){
					//RET
					case 0:{
							sp = bp+1;
							pc = stack[sp - 4];
							bp = stack[sp - 3];
							if(sp == 1000){
									halt = 1;
								}
							arr[ars--] = 0;
							break;
					}
					//NEG
					case 1: {
							-stack[sp];
							break;
					}
					// ADD
					case 2:{
							sp = sp+1;
							stack[sp] = stack[sp] + stack[sp-1];
							break;
					}
					//SUB
					case 3: {
							sp = sp+1;
							stack[sp]=stack[sp]-stack[sp-1];
							break;

					}
					//MUL
					case 4: {
							sp=sp+1;
							stack[sp]=stack[sp]*stack[sp-1];
							break;
					}
					//DIV
					case 5: {
							sp=sp+1;
							stack[sp]=stack[sp]/stack[sp-1];
							break;
					}
					//ODD
					case 6: {
							stack[sp] = stack[sp] % 2;
							break;
					}
					//MOD
					case 7: {
							sp=sp+1;
							stack[sp]=stack[sp] % stack[sp-1];
							break;
					}
					//EQL
					case 8: {
							sp=sp+1;
							stack[sp] = ((stack[sp] == stack[sp-1]) ? 1 : 0);
							break;
					}
					//NEQ
					case 9: {
							sp=sp+1;
							stack[sp] = ((stack[sp] != stack[sp-1]) ? 1 : 0);
							break;
					}
					//LSS
					case 10: {
							sp=sp+1;
							stack[sp] = ((stack[sp] < stack[sp-1]) ? 1 : 0);
							break;
					}
					//LEQ
					case 11: {
							sp=sp+1;
							stack[sp] = ((stack[sp] <= stack[sp-1]) ? 1 : 0);
							break;
					}
					//GTR
					case 12: {
							sp=sp+1;
							stack[sp] = ((stack[sp] > stack[sp-1]) ? 1 : 0);
							break;
					}
					//GEQ
					case 13: {
							sp=sp+1;
							stack[sp] = ((stack[sp] >= stack[sp-1]) ? 1 : 0);
							break;
					}
				}
				break;
				// LOD, this should load the value from location M from L levels up and push it to the top.
				case 3: {
						int l = ir.l;
						int b;
						while (l>0){
							b = stack[b-1];
							l--;
						}
						sp = sp - 1;
						stack[sp] = stack[base(l, bp) - ir.m];
						break;
				}
				// STO - take the value from the top of the stack and store it at location M from L levels up.
				case 4: {
					int l = ir.l;
					int s = bp;
					while (l>0){
						s = stack[s-1];
						l--;
					}
					stack[base(l, s) - ir.m] = stack[sp];
					sp = sp + 1;
					break;
				}
				// CALL
				case 5: {
					stack[sp - 1] = 0; // space to return value
					stack[sp-2] = base(ir.l, bp); // Static Link
					stack[sp-3] = bp; //Dynamic Link
					stack[sp-4] = pc; // Return Address
					bp = sp - 1;
					pc = ir.m;
					arr[ars++] = bp;
						break;
				}
				// INC
				case 6: {
					sp = sp - ir.m;
					break;
				}
				// JMP
				case 7: {
					pc = ir.m;
					break;
				}
				// JPC
				case 8: {
					if (stack[sp] == 0){
						pc = ir.m;
					}
					sp = sp +1;
					break;
				}
				// SIO
				case 9: {
					printf("%d", &stack[sp]);
					sp=sp+1;
					break;
				}
				// SIO 0, 2
				case 10: {
					sp=sp-1;
					scanf("%d", &stack[sp]);
					break;
				}
				// SIO 0, 3
				case 11: {
					halt=1;
					break;
				}
		}
	//write the pointers
	printStack(2, flag);

	//write the stack
	printStack(3, flag);
	}
}

int base(int l, int base) // l stand for L in the instruction format
{  
  int b1; //find base L levels down
  b1 = base; 
  while (l > 0)
  {
    b1 = stack[b1 - 1];   
    l--;
  }
  return b1;
}



void printStack(int status, int flag){
	int i=0, numAr = 0;
	char opcode[4];
	strcpy(opcode, getOpcode(ir));

	if(flag)
		switch(status){
			//write the instruction
			case 1: printf("%2d   %-5s%-5d%-5d", prev, opcode, ir.l, ir.m);
			break;

			//write the pointers
			case 2: printf("%-5d%-5d%-5d", pc, bp, sp);
			break;

			//write our formatted stack
			case 3: for(i = 999; i>=sp; i--){

				//dertermine if we need to print an activation record bar
				if(numAr < ars && i == arr[numAr] && i != 0){
					numAr++;
					printf( "| ");
				}

				//print our stack content
				printf( " %d ", stack[i]);
			}
			printf("\n");
			break;
		}
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
#endif VM.h
