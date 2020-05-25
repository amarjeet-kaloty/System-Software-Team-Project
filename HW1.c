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
int stack[MAX_DATA_STACK_HEIGHT];

struct instruction ir[10000];
int sp = MAX_DATA_STACK_HEIGHT; //stack pointer
int bp = sp - 1;  //base pointer
int pc = 0; //program counter




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
        sp = sp+1;
        stack[sp] = ir.m;

    }
    case 2: //OPR, so m must be evaluated.
        switch(ir.m)
        {

        case 0: //RET
            {
                sp = bp+1;
                pc = stack[sp - 4];
                bp = stackp[sp - 3];
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
                stack[sp] = (((stack[sp == stack[sp-1)]) ? 1 : 0);
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
        case 11: /LEQ
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



}






















int main()
{

    return 0;
}
