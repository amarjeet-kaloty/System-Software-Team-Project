#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_DATA_STACK_HEIGHT  1000
#define MAX_CODE_LENGTH  500
struct instruction
{
  int op; // op code
  int l; // lexicographical level
  int m; // depends on opcode.
};
struct instruction code[MAX_CODE_LENGTH];
int stack[MAX_DATA_STACK_HEIGHT] = {0};

struct instruction ir;
int sp = MAX_DATA_STACK_HEIGHT; //stack pointer
int bp = 999;  //base pointer
int pc = 0; //program counter
int haltflag = 0;
int ars = 0;
int prev=0;
int arBreak[2000] = {0};
int base(int l, int base){

	int b1; //find base L levels down
	b1 = base;
	while (l > 0){
		b1 = stack[b1 + 1];
	  	l--;
	}
	return b1;
}



void fetch()
{
    ir = code[pc];
    prev=pc;  // instruction goes into the instruction register.
    pc = pc + 1; //incrementing the program counter

}

void execute()
{
    switch(ir.op)
    {
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
                int b;
                while (l>0)
                {

                    b = stack[b-1];
                    l--;
                }
                stack[base(l, bp) - ir.m] = stack[sp];
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
        case 9: //SIO
            {
                printf("%d", &stack[sp]);
                sp=sp+1;
                break;

            }
        case 10: //SIO 0, 2
            {

                sp=sp-1;
                scanf("%d", &stack[sp]);
                break;
            }
        case 11: // SIO 0, 3
            {

                haltflag=0;
                break;
            }




}
}
char *getOp(struct instruction x){


	switch(x.op){

		case 1:
		return "LIT";

		//OPR
		case 2:
		switch(x.m){

			case 0:
			return "RET";
			case 1:
			return "NEG";
			case 2:
			return "ADD";
			case 3:
			return "SUB";
			case 4:
			return "MUL";
			case 5:
			return "DIV";
			case 6:
			return "ODD";
			case 7:
			return "MOD";
			case 8:
			return "EQL";
			case 9:
			return "NEQ";
			case 10:
			return "LSS";
			case 11:
			return "LEQ";
			case 12:
			return "GTR";
			case 13:
			return "GEQ";
		}

		case 3:
		return "LOD";
		case 4:
		return "STO";
		case 5:
		return "CAL";
		case 6:
		return "INC";
		case 7:
		return "JMP";
		case 8:
		return "JPC";
		case 9:
		return "SIO";
		case 10:
		return "SIO";
		case 11:
        return "SIO";
		default: return "";
	}
}


void RWInput(char in[], char out[]){

	int i = 0, x = 0;

	FILE *fp = fopen(in, "r");

	if(fp == NULL){
		printf("ERROR OPENING FILE %s!!\n", in);
	}

	//read in our initial instruction
	fscanf(fp, "%d%d%d", &code[i].op, &code[i].l, &code[i].m);

	//read in all additional instructions
	while(!feof(fp)){
		i++;
		fscanf(fp, "%d%d%d", &code[i].op, &code[i].l, &code[i].m);
	}

	fclose(fp);


	fp = fopen(out, "w");

	if(fp == NULL){
		printf("ERROR OPENING FILE %s!!\n", out);
	}

	//format and output line headers
	fprintf(fp, "%-5s  %-5s%-5s%-5s\n", "Line", "OP", "L", "M");

	//print out our correctly formatted instructions
	for(x = 0; x < i; x++){
		char op[4];
		strcpy(op, getOp(code[x]));
		fprintf(fp, "%4d  %-5s %-5d%-5d\n", x, op, code[x].l, code[x].m);
	}
	fprintf(fp, "\n\n");
	fprintf(fp,"                    %-5s%-5s%-5s%s\n", "pc", "bp", "sp", "stack");
}


void WriteToOutput(int status){

	int i=0, numAr = 0;
	char op[4];
	strcpy(op, getOp(ir));

	//determine what we need to write
	switch(status){

		//write the instruction
		case 1: printf("%2d   %-5s%-5d%-5d", prev, op, ir.l, ir.m);
		break;

		//write the pointers
		case 2: printf("%-5d%-5d%-5d", pc, bp, sp);
		break;

		//write our formatted stack
		case 3: for(i = bp; i >=sp; i--){

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




void Run(){
	//if(fp == NULL){
	//	printf("ERROR OPENING FILE %s!!\n", "stacktrace.txt");
	//}

	//print our initial values
	printf("Initial Values      %-5d%-5d%-5d\n", 0,999,1000);

	//run each instruction until we get to the halt condition
	while(!haltflag){

		//get instruction
		fetch();

		//write the instruction
		WriteToOutput(1);

		//ecevute the instruction
		execute();

		//write the pointers
		WriteToOutput(2);

		//write the stack
		WriteToOutput(3);
	}

	//fclose(fp);
}





















int main()
{
    RWInput("input.txt","output.txt");
    Run();


    return 0;
}
