#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//Declare program constraint constants.
#define IDENTIFIER_MAX_LENGTH 11
#define NUM_MAX_LENGTH 5
#define MAX_CODE_LENGTH 1000
#define MAX_ARRAY_SIZE 1000

typedef enum token_type{
    nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,  slashsym = 7, oddsym = 8,
    eqlsym = 9, neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16,
    commasym = 17, semicolonsym = 18, periodsym = 19, becomessym = 20, lbracesym = 21, rbracesym = 22, ifsym = 23,
    thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30, writesym = 31,
    readsym = 32, elsesym = 33
}token_type;

//Initialize input file character array and keep track of it's length.
char inputArray[MAX_CODE_LENGTH];
int input_length = 0;

//Declare input and output files
FILE *inputFile = NULL;

//Declare necessary arrays for tokens, numbers, and identifiers
int tokens[MAX_ARRAY_SIZE];
int token_cnt = 0;

int numbers[MAX_ARRAY_SIZE];
int numbers_cnt = 0;
int numbers_print = 0;

char identifiers_list[MAX_ARRAY_SIZE][IDENTIFIER_MAX_LENGTH];
int  identifiers_ints[MAX_ARRAY_SIZE];
int  identifiers_cnt = 0;

int characterToToken(char sym);
char* tokenToCharacter(int token);

void main(){
    //Open input
    inputFile = fopen("input.txt", "r");

    //If file not found throw an error
    if(inputFile == NULL){
        printf("\nError! File not found.\n");
        return;
    }

    //Read in the input file character-by-character.
    char temp;
    int i = 0;
    while(fscanf(inputFile, "%c", &temp) != EOF) {
        inputArray[i]= temp;
        i++;
        input_length++;
    }
    //Close input file.
    fclose(inputFile);

    //Output the Original Source Program Code
    printf("Source Program:\n");
    for (i=0; i<input_length; i++){
        printf("%c", inputArray[i]);
    }

    printf("\n");

    //Looping through the array
    for (i=0; i < input_length; i++){

        if (isdigit(inputArray[i]) == 1 && isalpha(inputArray[i+1]) != 0)
        {
            printf("\nError! Number then Letter Appeared which is invalid.");
            exit(EXIT_FAILURE);
        }

        if( inputArray[i] == '@' || inputArray[i] == '~' || inputArray[i] == '`' || inputArray[i] == '#'
            || inputArray[i] == '#' || inputArray[i] ==  '$' || inputArray[i] == '%'|| inputArray[i] == '^'
            || inputArray[i] == '&' || inputArray[i] == '_' || inputArray[i] == '?'
            || inputArray[i] == '"' || inputArray[i] == '[' || inputArray[i] == ']' ){
            printf("\nError! Invalid character");
            exit(EXIT_FAILURE);
        }

        //Check for :=
        if(inputArray[i] == ':'){
            int nextSymbol = characterToToken(inputArray[i+1]);
            if(nextSymbol == eqlsym){
                tokens[token_cnt] = becomessym;
                token_cnt++;
                i++;
                continue;
            }
            //Else, there is an invalid symbol.
            else{
                printf("\nError: Invalid Symbol.");
                exit(EXIT_FAILURE);
            }
        }

        //Check for '!='
        if(inputArray[i] == '!'){
            int nextSymbol = characterToToken(inputArray[i+1]);
            if(nextSymbol == eqlsym){
                tokens[token_cnt] = neqsym;
                token_cnt++;
                i++;
                continue;
            }
            //Else, there is an invalid symbol.
            else{
                printf("\nError: Invalid Symbol.");
                exit(EXIT_FAILURE);
            }
        }
        

        //For Comments
        if (inputArray[i] == '/' && inputArray[i+1] == '*'){
            int j=i+2;

            while (!(inputArray[j] == '*' && inputArray[j+1] == '/'))
            {
                j++;
            }
            i=j+2;
            continue;
        }

        //For newline, whitespace, and tab
        if (inputArray[i] == '\n')
            continue;

        if (inputArray[i] == ' ')
            continue;

        if (inputArray[i] == '\t')
            continue;

        if (inputArray[i] >= 'a' && inputArray[i] <= 'z'){
            switch ((int) inputArray[i]){
               //Check for 'call'
                case (int) 'c':
                    if ((inputArray[i+1] == 'a') &&
                        (inputArray[i+2] == 'l') &&
                        (inputArray[i+3] == 'l'))
                    {
                        i+=3;
                        tokens[token_cnt] = callsym;
                        token_cnt++;
                        continue;
                    }
                    //check for 'const'
                    else if ((inputArray[i+1] == 'o') &&
                             (inputArray[i+2] == 'n') &&
                             (inputArray[i+3] == 's') &&
                             (inputArray[i+4] == 't'))
                    {
                        i+=4;
                        tokens[token_cnt] = constsym;
                        token_cnt++;
                        continue;
                    }

                //Check for 'var'
                case (int) 'v':
                    if((inputArray[i+1] == 'a') &&
                       (inputArray[i+2] == 'r'))
					{
						i+=2;
						tokens[token_cnt] = varsym;
						token_cnt++;
						continue;
					}

                //Check for 'procedure
                case (int) 'p':
                    if ((inputArray[i+1] == 'r') &&
                        (inputArray[i+2] == 'o') &&
                        (inputArray[i+3] == 'c') &&
                        (inputArray[i+4] == 'e') &&
                        (inputArray[i+5] == 'd') &&
                        (inputArray[i+6] == 'u') &&
                        (inputArray[i+7] == 'r') &&
                        (inputArray[i+8] == 'e'))
                    {
                        i+=8;
                        tokens[token_cnt] = procsym;
                        token_cnt++;
                        continue;
                    }

                //Check for 'if'
                case (int) 'i':
                    if (inputArray[i+1] == 'f')
                    {
                        i+=1;
                        tokens[token_cnt] = ifsym;
                        token_cnt++;
                        continue;
                    }

                //Check for 'then'
                case (int) 't':
                    if ((inputArray[i+1] == 'h') &&
                        (inputArray[i+2] == 'e') &&
                        (inputArray[i+3] == 'n'))
                    {
                        i+=3;
                        tokens[token_cnt] = thensym;
                        token_cnt++;
                        continue;
                    }


 				//Check for 'else'
                case (int) 'e':
                    if ((inputArray[i+1] == 'l') &&
                        (inputArray[i+2] == 's') &&
                        (inputArray[i+3] == 'e'))
                    {
                        i+=3;
                        tokens[token_cnt] = elsesym;
                        token_cnt++;
                        continue;
                    }

                case (int) 'w':
                    //Check for 'while'
                    if ((inputArray[i+1] == 'h') &&
                        (inputArray[i+2] == 'i') &&
                        (inputArray[i+3] == 'l') &&
                        (inputArray[i+4] == 'e'))
                    {
                        i+=4;
                        tokens[token_cnt] = whilesym;
                        token_cnt++;
                        continue;
                    }
                    //Check for 'write'
                    else if ((inputArray[i+1] == 'r') &&
                             (inputArray[i+2] == 'i') &&
                             (inputArray[i+3] == 't') &&
                             (inputArray[i+4] == 'e'))
                    {
                        i+=4;
                        tokens[token_cnt] = writesym;
                        token_cnt++;
                        continue;
                    }

                //Check for 'do'
                case (int) 'd':
                    if (inputArray[i+1] == 'o')
                    {
                        i+=1;
                        tokens[token_cnt] = dosym;
                        token_cnt++;
                        continue;
                    }

                //Check for 'read'
                case (int) 'r':
                    if ((inputArray[i+1] == 'e') &&
                        (inputArray[i+2] == 'a') &&
                        (inputArray[i+3] == 'd'))
                    {
                        i+=3;
                        tokens[token_cnt] = readsym;
                        token_cnt++;
                        continue;
                    }

                //Check for 'odd'
                case (int) 'o':
                    if ((inputArray[i+1] == 'd') &&
                        (inputArray[i+2] == 'd'))
                    {
                        i+=2;
                        tokens[token_cnt] = oddsym;
                        token_cnt++;
                        continue;
                    }
                
                default:
                    break;
            }
        }

        //Takes a character and converts it into a token and stores it in the token array
        temp = characterToToken(inputArray[i]);
        if (temp != 0){
			int nextSymbol;
			//Check for <> and <=
			if(temp == lessym){
                printf("in here\n");
				nextSymbol = characterToToken(inputArray[i+1]);
				if(nextSymbol == gtrsym){
					tokens[token_cnt] = neqsym;
					token_cnt++;
					i++;
					continue;
				}
				else if(nextSymbol == eqlsym){
					tokens[token_cnt] = leqsym;
					token_cnt++;
					i++;
					continue;
				}
			}

			//Check for >=
			else if(temp == gtrsym){
				nextSymbol = characterToToken(inputArray[i+1]);
				if(nextSymbol == eqlsym){
					tokens[token_cnt] = geqsym;
					token_cnt++;
					i++;
					continue;
				}
			}

            nextSymbol = characterToToken(inputArray[i+1]);

            //Save the token we just got.
            tokens[token_cnt] = temp;
            token_cnt++;

            continue;
        }

        //Reads in identifiers to id_list
        if ((inputArray[i] >= 'A' && inputArray[i] <= 'Z') || (inputArray[i] >= 'a' && inputArray[i] <= 'z')){
            int flag= 1;
			int id_length;
            char id[IDENTIFIER_MAX_LENGTH];
            int j, k, l;
            int  identifiers_total_number = 1;
            for (k=0; k<IDENTIFIER_MAX_LENGTH; k++)
                id[k]= '\0';

            id_length=0;
            j=i;
            while((inputArray[j] >= 'A' && inputArray[j] <= 'Z') ||
                  (inputArray[j] >= 'a' && inputArray[j] <= 'z') ||
                  (inputArray[j] >= '0' && inputArray[j] <= '9'))
            {
                //If identifier is too long throw an error and exit
                if (id_length > 11)
                {
                    printf("\nError: Invalid Identifier!");
                    exit(EXIT_FAILURE);
                }

                //otherwise store the char inside a new string for an identifier
                id[id_length] = inputArray[j];
                id_length++;
                j++;
            }

            //store the identifier in token array
            tokens[token_cnt]= identsym;
            token_cnt++;

            for (l=0; l<identifiers_cnt; l++)
            {
                if (strcmp(id, identifiers_list[l]) == 0)
                {
                    identifiers_ints[identifiers_cnt]= l+1;
                    flag= 0;
                    break;
                }
            }
            if (flag)
            {
                identifiers_ints[identifiers_cnt]= identifiers_total_number;
                identifiers_total_number++;
            }
            strcpy(identifiers_list[identifiers_cnt],id);
            identifiers_cnt++;
            i= j-1;
            continue;
        }

        //For Numbers
        if (inputArray[i] >= '0' && inputArray[i] <= '9')
        {
			int digits;
			int number;
            int j= i;
			int k;
            //Finds length of the current number
            for (j= i; j< NUM_MAX_LENGTH+i+1 ; j++)
            {

                //To determine the end of a number
                if ((inputArray[j] == ' ') || (inputArray[j] == '\n') ||
                    (inputArray[j] == '\t'))
                    break;


            }

             //Set j to where we were from i.
            j=i;

            //We know we have one digit, so count on to see if the number of digits is too large.
            digits=1;
            number=0;
            while (inputArray[j] >= '0' && inputArray[j] <= '9')
            {
                if (digits > 5)
                {
                    printf("\nError: Number too long.");
                    exit(EXIT_FAILURE);
                }
                digits++;
                j++;
            }

            //Get digits back to 1s place
            digits--;

            for (k= j-digits; k<j; k++)
            {
                number += (inputArray[k]- '0') * pow(10., digits-1);
                digits--;
            }

            //Save that number to our list
            tokens[token_cnt]= numbersym;
            token_cnt++;
            numbers[numbers_cnt]= number;
            numbers_cnt++;

            i= j-1;
            continue;
        }

    }

	int num_count= 0;
	int identifiers_cnt= 0;
    printf("Lexeme Table:\n");
    printf("Lexeme      Token Type\n");
    char* lexeme = (char*)malloc(100);
    //Print Lexeme Table and Tokens
	for (i=0; i< token_cnt; i++)
	{
		if (tokens[i]== numbersym)
		{
			printf("%d\t\t\t%d\n", numbers[num_count], tokens[i]);
			num_count++;
		}
		else if (tokens[i]== identsym)
		{
			printf("%s\t\t\t%d\n", identifiers_list[identifiers_cnt], tokens[i]);
            identifiers_cnt++;
		}
		else
        {   strcpy(lexeme, tokenToCharacter(tokens[i]));
			printf("%s\t\t\t%d\n", lexeme, tokens[i]);
        }
	}

    identifiers_cnt= 0;
    int cnt = 0;
    int k = 0;
    int symflag = 0;
    char* temp_id = (char*)malloc(100);
    //Print the Lexeme List.
    printf("\nLexeme List:\n");
    for (i=0; i< token_cnt; i++)
	{   symflag = 0;
		if (tokens[i]== numbersym)
		{
		    for(int j=0; j<cnt; j++){
                printf("%d %d ", tokens[i], numbers[k]);
                cnt++;
                symflag = 1;
                k++;
                break;
		    }

		}
		else if (tokens[i]== identsym)
		{
		    for(int j=0; j<cnt; j++){
                printf("%d %s ", tokens[i], strcpy(temp_id, identifiers_list[identifiers_cnt]));
                cnt++;
                identifiers_cnt++;
                symflag=1;
                break;
            }
		    if(symflag==0){
                printf("%d %s ", tokens[i], strcpy(temp_id, identifiers_list[identifiers_cnt]));
                cnt++;
                identifiers_cnt++;
		    }
		}
		else
        {
            printf("%d ", tokens[i]);
        }
	}
    return;

}

//This function converts a character to a token represented by an integer in enum.
int characterToToken(char sym)
{
    switch ((int) sym)
    {

        case (int) '*':
            return multsym;

        case (int) '+':
            return plussym;

        case (int) '-':
            return minussym;

        case (int) '/':
            return slashsym;

        case (int) '(':
            return lparentsym;

        case (int) ')':
            return rparentsym;

        case (int) ',':
            return commasym;

        case (int) ';':
            return semicolonsym;

        case (int) '.':
            return periodsym;

        case (int) '<':
            return lessym;

        case (int) '>':
            return gtrsym;

        case (int) '=':
            return eqlsym;

        case (int) '{':
            return lbracesym;

        case (int) '}':
            return rbracesym;

        default:
            return 0;
    }
    return 0;
}

//This function converts a token to a character.
char* tokenToCharacter(int token){
    char* temp;
    switch (token)
    {
        case (int) multsym:
            return "*";

        case (int) plussym:
            return "+";

        case (int) minussym:
            return "-";

        case (int) slashsym:
            return "/";

        case (int) lessym:
            return "<";

        case(int) eqlsym:
            return "=";

        case (int) gtrsym:
            return ">";

		case(int) geqsym:
			return ">=";

		case(int) leqsym:
			return "<=";

		case(int) becomessym:
			return ":=";

		case(int) neqsym:
			return "<>";

        case (int) lparentsym:
            return "(";

        case (int) rparentsym:
            return ")";

        case (int) commasym:
            return ",";

        case (int) semicolonsym:
            return ";";

        case (int) periodsym:
            return ".";

        case (int) varsym:
            return "var";

        case (int) lbracesym:
            return "{";

        case (int) rbracesym:
            return "}";

        case (int) ifsym:
            return "if";

        case (int) thensym:
            return "then";

        case (int) whilesym:
            return "while";

        case (int) dosym:
            return "do";

        case (int) callsym:
            return "call";

        case (int) constsym:
            return "const";

        case (int) elsesym:
            return "else";

        case (int) procsym:
            return "procedure";

        case (int) readsym:
            return "read";

        case (int) writesym:
            return "write";

        case (int) oddsym:
            return "odd";

        case (int) nulsym:
            return NULL;

        default:
            return 0;
    }
}
