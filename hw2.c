#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//Declare program constraint constants.
#define IDENTIFIER_MAX_LENGTH   11
#define NUM_MAX_LENGTH          5
#define SYMBOL_TABLE_SIZE   500

//Data structure for the symbol.
typedef struct symbols{
    int kind;       //const = 1, var = 2, proc = 3
    char name[11];  //name up to 11 chars
    int val;        //number (ASCII value)
    int level;      //L Level
    int addr;       //M address
}symbols;

//Create a symbol table.
symbols ST[SYMBOL_TABLE_SIZE];

typedef struct name_list{
    char name[11]; //keep track of symbols before we make a symbol table.
}name_list;

typedef enum token_type{
nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,  slashsym = 7, oddsym = 8,
eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14,
lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19,
becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24, whilesym = 25, dosym = 26,
callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31, readsym = 32, elsesym = 33, colonsym = 34
}token_type;

//Initialize input file character array and keep track of it's length.
char in[20000];
int input_length = 0;

//Declare input and output files
FILE *ifp = NULL;
FILE *ofp;

//Declare necessary arrays for tokens, numbers, and identifiers
int tokens[20000];
int token_cnt =  0;

int num[20000];
int num_cnt = 0;
int num_print= 0;

char id_list[20000][IDENTIFIER_MAX_LENGTH];
int  id_ints[20000];
int  id_cnt = 0;
int  id_total_number = 1;

//Declare functions for converting strings to tokens and tokens to strings.
int characterToToken(char sym);
char* tokenToCharacter(int token);

void main(int print){

    //Read in filename from command line argument.
    //char* fname = argv[1];

    //Open input and output files
    ifp =   fopen("input.txt", "r");
    ofp =   fopen("output.txt", "w");

    //If the file isn't there or could not open properly, end the program.
    if(ifp == NULL){
        printf("File could not open. Program halted.\n");
        return;
    }

    //Read in the input file character by character.
    char temp;
    int i = 0;
    while(fscanf(ifp,"%c", &temp) != EOF) {
        in[i]= temp;
        i++;
        input_length++;
    }
    //Close input file.
    fclose(ifp);

    printf("Source Program: \n");
    for(i = 0; i < input_length; i++){
        printf("%c", in[i]);
    }
}
