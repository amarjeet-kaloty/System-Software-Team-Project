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
int code[MAX_CODE_LENGTH];
int stack[MAX_DATA_STACK_HEIGHT];

struct instruction ir[10000];
int sp = MAX_DATA_STACK_HEIGHT; //stack pointer
int bp = sp - 1;  //base pointer
int pc = 0; //program counter
int haltflag;
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
    ir = code[pc]; // instruction goes into the instruction register.
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
                stack[sp] = stack[base(l, b) - ir.m];
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
                stack[base(l, b) - ir.m] = stack[sp];
                sp = sp + 1;

                break;

            }
        case 5: // CALL at M
            {
                stack[sp-1] = 0;
                stack[sp-2] = base(ir.l, bp);
                stack[sp-3] = bp;
                stack[sp-4] = pc;
                bp = sp-1;
                pc = ir.m;
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


void readInput(char in[], char out[])
{
    
    int i = 0, x = 0;
    FILE *fp;
    fp = fopen("input.txt", "r");
    
    if (fp == NULL)
    {
        
        printf("Error opening file");
    }
    fscanf(fp, "%d%d%d", &code[i].op, &code[i].l, &code[i].m);
    
    
}
























int main()
{

    return 0;
}
