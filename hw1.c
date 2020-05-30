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
void WriteToOutput(int x);
int base(int x, int base);
void execute();

instruction code[MAX_CODE_LENGTH];
int stack[MAX_DATA_STACK_HEIGHT] = {0};
instruction ir;
int sp = MAX_DATA_STACK_HEIGHT; //stack pointer
int bp = 999;  //base pointer
int pc = 0; //program counter
int halt = 0;
int arBreak[2000] = {0};
int ars = 0;
int prev = 0;

int main(){
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

	printf("Initial Values      %-5d%-5d%-5d\n", 0,999,1000);

	while(!0){
		
		//Fetch
		ir = code[pc];
    	prev = pc;  // instruction goes into the instruction register.
    	pc = pc + 1; //incrementing the program counter

		//write the instruction
		WriteToOutput(1);
		
		execute();

		WriteToOuput(2);

		WriteToOutput(3);
	}
	return 0;
}

void execute(){
    switch(ir.op){
    case 1: //LIT
        sp = sp - 1;
        stack[sp] = ir.m;
	break;

    case 2: //OPR, so m must be evaluated.
        switch(ir.m)
        {

        case 0: //RET
            {
                sp = bp+1;
                pc = stack[sp - 4];
                bp = stack[sp - 3];
                if(sp == 1000){
						halt = 1;
					}
                arBreak[ars--] = 0;

                break;

            }
        case 1: // NEG
            {
                -stack[sp];
                break;
            }
        case 2: // ADD
            {
                sp = sp+1;
                stack[sp] = stack[sp] + stack[sp-1];
                break;
            }
        case 3: // SUB
            {
                sp = sp+1;
                stack[sp]=stack[sp]-stack[sp-1];
                break;

            }
        case 4: //MUL
            {
                sp=sp+1;
                stack[sp]=stack[sp]*stack[sp-1];
                break;
            }
        case 5: // DIV
            {
                sp=sp+1;
                stack[sp]=stack[sp]/stack[sp-1];
                break;
            }
        case 6: // ODD
            {
                stack[sp] = stack[sp] % 2;
                break;
            }
        case 7: // MOD
            {
                sp=sp+1;
                stack[sp]=stack[sp] % stack[sp-1];
                break;
            }
        case 8: //EQL
            {
                sp=sp+1;
                stack[sp] = ((stack[sp] == stack[sp-1]) ? 1 : 0);
                break;
            }
        case 9: //NEQ
            {
                sp=sp+1;
                stack[sp] = ((stack[sp] != stack[sp-1]) ? 1 : 0);
                break;
            }
        case 10: //LSS
            {
                sp=sp+1;
                stack[sp] = ((stack[sp] < stack[sp-1]) ? 1 : 0);
                break;
            }
        case 11: //LEQ
            {
                sp=sp+1;
                stack[sp] = ((stack[sp] <= stack[sp-1]) ? 1 : 0);
                break;
            }
        case 12: // GTR
            {
                sp=sp+1;
                stack[sp] = ((stack[sp] > stack[sp-1]) ? 1 : 0);
                break;
            }
        case 13: //GEQ
            {
                sp=sp+1;
                stack[sp] = ((stack[sp] >= stack[sp-1]) ? 1 : 0);
                break;
            }
        }
        break;

        case 3: // LOD, this should load the value from location M from L levels up and push it to the top.
            {
                int l = ir.l;
                int b;
                while (l>0)
                {

                    b = stack[b-1];
                    l--;
                }
                sp = sp - 1;
                stack[sp] = stack[base(l, bp) - ir.m];
                break;
            }
        case 4:  // STO - take the value from the top of the stack and store it at location M from L levels up.
            {
                int l = ir.l;
                int s = bp;
                while (l>0)
                {

                    s = stack[s-1];
                    l--;
                }
                stack[base(l, s) - ir.m] = stack[sp];
                sp = sp + 1;

                break;

            }
        case 5: // CALL at M
            {
                stack[sp - 1] = 0; // space to return value
                stack[sp-2] = base(ir.l, bp); // Static Link
                stack[sp-3] = bp; //Dynamic Link
                stack[sp-4] = pc; // Return Address
               bp = sp - 1;
               pc = ir.m;
               arBreak[ars++] = bp;
                break;
            }
        case 6: // INC
            {
                sp = sp - ir.m;
                break;
            }
        case 7: // JMP
            {
                pc = ir.m;
                break;
            }
        case 8: // JPC
            {
                if (stack[sp] == 0)
                {
                    pc = ir.m;
                }
                sp = sp +1;
                break;
            }
        case 9:
            switch(ir.m) {
                case 1: //SIO
            {
                printf("%d", &stack[sp]);
                sp=sp+1;
                break;
            }
        case 2: //SIO 0, 2
            {
                sp=sp-1;
                scanf("%d", &stack[sp]);
                break;
            }
        case 3: // SIO 0, 3
            {

                halt=1;
                break;
            }
        }
    }
}

int base(int l, int base){

	int b1; //find base L levels down
	b1 = base;
	while (l > 0){
		b1 = stack[b1 + 1];
	  	l--;
	}
	return b1;
}

void WriteToOutput(int status){

	int i=0, numAr = 0;
	char op[4];
	strcpy(op, getOpcode(ir));

	//determine what we need to write
	switch(status){

		//write the instruction
		case 1: printf("%2d   %-5s%-5d%-5d", prev, op, ir.l, ir.m);
		break;

		//write the pointers
		case 2: printf("%-5d%-5d%-5d", pc, bp, sp);
		break;

		//write our formatted stack
		case 3: for(i = 999; i >=sp; i--){

			//dertermine if we need to print an activation record bar
			if(numAr < ars && i == arBreak[numAr] && i != 0){
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
