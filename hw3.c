#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct code{
	int op;
	int l;
	int m;
}code;

typedef struct symbol {
    char* id;
    int type; //0 - const, 1 - variable, 2 - procedure
    int value;
    int position;
    int level;

    struct symbol *next;
} symbol;

typedef struct symbolTable{
    int size;       /* the size of the table */
    symbol **symbols; /* the table elements */
} symbolTable;

FILE* fp;
symbolTable* st;
code* codegen;

char* token;
char** input;
int tptr = 0;
int line = 0;
int offset = 4;
int lex = 0;
int stacksize = 0;

void Expression();
void Term();
void Factor();
void PList();

//Creates our symbol table
symbolTable* CreateTable(int size){

	int i;

    symbolTable *newTable;

    if (size<1) return NULL; //invalid size for table

    //Attempt to allocate memory for the table structure
    if ((newTable = malloc(sizeof(symbolTable))) == NULL) {
        return NULL;
    }

    //Attempt to allocate memory for the table itself
    if ((newTable->symbols = malloc(sizeof(symbol*) * size)) == NULL) {
        return NULL;
    }

    //Initialize the elements of the table
    for(i=0; i<size; i++) newTable->symbols[i] = NULL;

    //Set the table's size
    newTable->size = size;

    return newTable;
}

//Our hash function
unsigned int Hash(symbolTable* hashtable, char* str){

    unsigned int hashval;

    hashval = 0;

    //for each character, we multiply the old hash by 31 and add the current
    //character.
    for(; *str != '\0'; str++) hashval = *str + (hashval << 5) - hashval;

    return hashval % hashtable->size;
}

//Given the table and the name of our string, return a pointer
//to that symbol in the table
symbol* Lookup(symbolTable* hashtable, char* str){

    symbol *list;

    unsigned int hashval = Hash(hashtable, str);

     //Go to the correct list based on the hash value and see if str is
     //in the list.  If it is, return return a pointer to the list element.
     //If it isn't, the item isn't in the table, so return NULL.
    for(list = hashtable->symbols[hashval]; list != NULL; list = list->next) {
        if (strcmp(str, list->id) == 0 && list->level <= lex) return list;
    }
    return NULL;
}

//Add to our symbol table
int Add(symbolTable* hashtable, char* id, int type, int value, int position, int level){

    symbol* new_list;
    symbol* current_list;
    unsigned int hashval = Hash(hashtable, id);

    //Attempt to allocate memory for list
    if ((new_list = malloc(sizeof(symbol))) == NULL) return 1;


    //Insert into list
    new_list->id = strdup(id);
    new_list->type = type;
    new_list->value = value;
    new_list->position = position;
    new_list->level = level;
    new_list->next = hashtable->symbols[hashval];

    hashtable->symbols[hashval] = new_list;


    return 0;
}


void Delete(symbolTable* hashtable){

	int i;
	symbol *list, *prev, *head, *tmp;

	for(i=0; i<hashtable->size; i++) {
        head = hashtable->symbols[i];
        list = head;
        while(list != NULL && lex <= list->level){
        	prev = list;
        	list = list->next;
        	head = list;
        	free(prev);
        }
        if(list == NULL){
        	hashtable->symbols[i] = head;
        	continue;
        }

        head = list;
        prev = list;
        list = list->next;


        while(list!=NULL){
        	if(list->next != NULL && lex <= list->level){
	        	prev->next = list->next;
	        	tmp = list;
	        	list = list->next;
	        	free(tmp);
	        }
	        else if(lex <= list->level){
	        	prev->next = NULL;
	        	tmp = list;
	        	list = list->next;
	        	free(tmp);
	        }
	        else{
	        	prev = list;
	        	list = list->next;
	        }
        }

        hashtable->symbols[i] = head;
    }
}



void PrintTable(symbolTable* table){

	printf("%d\n", lex);

	int i;
	symbol* list;

	for(i = 0; i < table->size; i++){
		list = table->symbols[i];
		while(list != NULL){
			printf("%s %d %d\n", list->id, list->level, i);
			list = list->next;
		}
	}

	printf("done\n");
}

//Initialize everything
void Initialize2(char in[], char out[]){

	int tokens = 0;
	int i = 0;
	char tmp;

	//count the number of tokens in file for proper allocation
	fp = fopen(in, "r");
	while(!feof(fp)){
		tmp = fgetc(fp);
		if(tmp == ' ') tokens++;
	}

	//allocate space for our input and token variable
	token = malloc(sizeof(char*)* 12);
	input = malloc(sizeof(char*) * tokens+2);
	for(i = 0; i < tokens+2; i++){
		input[i] = malloc(sizeof(char) * 12);
	}

	fclose(fp);
	fp = fopen(in, "r");

	//read our rokens
	for(i = 0; i < tokens; i++){
		fscanf(fp, "%s", input[i]);
	}

	fclose(fp);
	fp = fopen(out, "w");

	//create our symbol table and our code list
	int tableSize = 100;
	st = CreateTable(tableSize);
	codegen = malloc(sizeof(code*) * (tableSize * 10));

}

void UpdateStackSize(int op, int l, int m){

	if(op == 1 || op == 3 || op == 10){
		stacksize++;
	}
	else if(op == 4 || op == 8 || op == 9){
		stacksize--;
	}
	else if(op == 6){
		stacksize += m;
	}
	else if(op == 2){
		if(m == 0){
			stacksize = 0;
		}
		else if(m != 1 && m != 6){
			stacksize--;
		}
	}

}

//add an instruction to our code list
void Gen(int op, int l, int m){
	codegen[line].op = op;
	codegen[line].l = l;
	codegen[line++].m = m;
	UpdateStackSize(op, l, m);
}

//print our generated code to a file
void printOut(){
	int i;
	for(i=0; i<line; i++){
		fprintf(fp, "%d %d %d\n",codegen[i].op, codegen[i].l, codegen[i].m);
	}
}





//Get the next token and store it in token
void Get(){

	char* tmp = input[tptr++];

		if(!strcmp(tmp, "1")) token = "\0";

		else if(!strcmp(tmp, "2")) token = "2";

		else if(!strcmp(tmp, "3")) token = "3";

		else if(!strcmp(tmp, "4")) token = "+";

		else if(!strcmp(tmp, "5")) token = "-";

		else if(!strcmp(tmp, "6")) token = "*";

		else if(!strcmp(tmp, "7")) token = "/";

		else if(!strcmp(tmp, "8")) token = "odd";

		else if(!strcmp(tmp, "9")) token = "=";

		else if(!strcmp(tmp, "10")) token = "<>";

		else if(!strcmp(tmp, "11")) token = "<";

		else if(!strcmp(tmp, "12")) token = "<=";

		else if(!strcmp(tmp, "13")) token = ">";

		else if(!strcmp(tmp, "14")) token = ">=";

		else if(!strcmp(tmp, "15")) token = "(";

		else if(!strcmp(tmp, "16")) token = ")";

		else if(!strcmp(tmp, "17")) token = ",";

		else if(!strcmp(tmp, "18")) token = ";";

		else if(!strcmp(tmp, "19")) token = ".";

		else if(!strcmp(tmp, "20")) token = ":=";

		else if(!strcmp(tmp, "21")) token = "begin";

		else if(!strcmp(tmp, "22")) token = "end";

		else if(!strcmp(tmp, "23")) token = "if";

		else if(!strcmp(tmp, "24")) token = "then";

		else if(!strcmp(tmp, "25")) token = "while";

		else if(!strcmp(tmp, "26")) token = "do";

		else if(!strcmp(tmp, "27")) token = "call";

		else if(!strcmp(tmp, "28")) token = "const";

		else if(!strcmp(tmp, "29")) token = "var";

		else if(!strcmp(tmp, "30")) token = "procedure";

		else if(!strcmp(tmp, "31")) token = "write";

		else if(!strcmp(tmp, "32")) token = "read";

		else if(!strcmp(tmp, "33")) token = "else";

		else;

}

//Given the error code, report the error and terminate the program
void Error(int code){

	switch(code){

		case 1:
		printf("Use = instead of :=.\n");
		exit(0);

		case 2:
		printf("= must be followed by a number.\n");
		exit(0);

		case 3:
		printf("Identifier must be followed by =.\n");
		exit(0);

		case 4:
		printf("'const', 'var', 'procedure' must be followed by identifier.\n");
		exit(0);

		case 5:
		printf("Semicolon or comma missing.\n");
		exit(0);

		case 6:
		printf("Incorrect symbol after procedure declaration.\n");
		exit(0);

		case 7:
		printf("Statement expected.\n");
		exit(0);

		case 8:
		printf("Incorrect symbol after statement part in block.\n");
		exit(0);

		case 9:
		printf("Period expected.\n");
		exit(0);

		case 10:
		printf("Semicolon between statements missing.\n");
		exit(0);

		case 11:
		printf("Undeclared identifier.\n");
		exit(0);

		case 12:
		printf("Assignment to constant or procedure is not allowed.\n");
		exit(0);

		case 13:
		printf("Assignment operator expected.\n");
		exit(0);

		case 14:
		printf("'call' must be followed by an identifier.\n");
		exit(0);

		case 15:
		printf("Call of a constant or variable is meaningless.\n");
		exit(0);

		case 16:
		printf("'then' expected.\n");
		exit(0);

		case 17:
		printf("Semicolon or } expected.\n");
		exit(0);

		case 18:
		printf("do expected.\n");
		exit(0);

		case 19:
		printf("Incorrect symbol following statement.\n");
		exit(0);

		case 20:
		printf("Relational operator expected.\n");
		exit(0);

		case 21:
		printf("Expression must not contain a procedure identifier.\n");
		exit(0);

		case 22:
		printf("Right parenthesis missing.\n");
		exit(0);

		case 23:
		printf("The preceding factor cannot begin with this symbol.\n");
		exit(0);

		case 24:
		printf("An expression cannot begin with this symbol.\n");
		exit(0);

		case 25:
		printf("This number is too large.\n");
		exit(0);
	}
}

//parse an expression
void Expression(){

	int sym = 0;
	int yee = 0;

	//+ or - is optional at the begining of an expression
	if(!strcmp(token, "+") || !strcmp(token, "-")){

		if(!strcmp(token, "-")) yee = 1;

		sym = 1;
		Get();
		Term();

		if(yee) Gen(2,0,1);

	}

	//expression must have at least a single term
	//and can be followed by +/- and more terms
	while(sym == 0 || (!strcmp(token, "+") || !strcmp(token, "-"))){

		sym = 1;

		if(!strcmp(token, "+")){
			Get();
			Term();
			Gen(2,0,2);
		}
		else if(!strcmp(token, "-")){
			Get();
			Term();
			Gen(2,0,3);
		}
		else {
			Term();
		}


	}
}

//parse a factor
void Factor(){

	symbol* curSym;
	char* ident = malloc(sizeof(char*) * 12);
	int number;
	int tmp;

	//our factor is an ident
	if(!strcmp(token, "2")){

		//get the specific identifier
		ident = input[tptr++];

		//get the ident from our table
		curSym = Lookup(st, ident);

		//ident was not in table
		if(curSym == NULL){
			Error(11);
		}
		//wrong type of ident
		else if(curSym->type == 2){
			Error(21);
		}
		else{
			//push our constant
			if(curSym->type == 0){
				Gen(1,0,curSym->value);
			}
			else if(curSym->type == 1){
				//undefined variable
				if(curSym->position == -1)
					Error(11);
				//load variable value
				else Gen(3,(lex - curSym->level),curSym->position);
			}
		}
		Get();
	}

	//our factor is a number
	else if(!strcmp(token, "3")){

		//get the specific number and push it
		number = atoi(input[tptr++]);
		Gen(1,0,number);
		Get();
	}

	//our factor is in the form ( expresson )
	else if(!strcmp(token, "(")){

		Get();
		Expression();
		if(!strcmp(token, ")")) Get();
		else Error(22);
	}


	//call a procedure
	else if(!strcmp(token, "call")){
		Get();
		if(!strcmp(token, "2")){
			ident = input[tptr++];

			curSym = Lookup(st, ident);
			if(curSym == NULL){
				Error(11);
			}
			else if(curSym->type != 2){
				Error(15);
			}
			else{
				PList();
				Gen(5, (lex - curSym->level), curSym->position);
				Gen(6,0,1);
			}
		}
		else Error(14);

		Get();
	}

	else {
		Error(23);
	}
}

//parse a term
void Term(){

	int sym;

	//a term must start with a factor, parse it
	Factor();

	//we can have multiple factors multiplied or divided together
	while(!strcmp(token, "*") || !strcmp(token, "/")){

		if(!strcmp(token, "*")) sym = 4;
		else sym = 5;

		Get();
		Factor();
		Gen(2,0,sym);
	}
}

//parse a condition
void Condition(){

	int sym;

	//odd condition
	if(!strcmp(token, "odd")){
		Get();
		Expression();
		Gen(2,0,6);
	}

	//expression rel-op expression
	else{

		//parse the expression
		Expression();
		if(!strcmp(token, "=") || !strcmp(token, "<>") || !strcmp(token, "<")
			|| !strcmp(token, "<=") || !strcmp(token, ">") || !strcmp(token, ">=")){

			//decide which rel-op we have
			if(!strcmp(token, "=")) sym = 8;
			else if(!strcmp(token, "<>")) sym = 9;
			else if(!strcmp(token, "<")) sym = 10;
			else if(!strcmp(token, "<=")) sym = 11;
			else if(!strcmp(token, ">")) sym = 12;
			else if(!strcmp(token, ">=")) sym = 13;


			Get();
			//parse the next expression
			Expression();
			Gen(2,0,sym);

		}
		else Error(20);
	}
}

//parse a statement
void Statement(){

	char* ident = malloc(sizeof(char*) * 12);
	int number;
	int tmp;
	int jaddr;
	int caddr;
	int jaddr2;

	symbol* curSym;

	//ident
	if(!strcmp(token, "2")){

		//get the specific ident
		ident = input[tptr++];

		Get();
		//ident := expression
		if(!strcmp(token, ":=")) Get();
		else Error(11);

		//get our ident from the table
		//make sure our ident is a variable
		curSym = Lookup(st, ident);
		if(curSym == NULL){
			Error(11);
		}
		else if(curSym->type != 1){
			Error(12);
		}
		else{
			//if the variable has not yet been assigned,
			//assign its position on the stack
			if(curSym->position == -1){
				curSym->position = offset++;
			}

			//parse the expression and store the rseult
			Expression();
			Gen(4,(lex - curSym->level),curSym->position);
		}

	}

	//call a procedure
	else if(!strcmp(token, "call")){
		Get();
		if(!strcmp(token, "2")){
			ident = input[tptr++];

			curSym = Lookup(st, ident);
			if(curSym == NULL){
				Error(11);
			}
			else if(curSym->type != 2){
				Error(15);
			}
			else{
				PList();
				Gen(5, (lex - curSym->level), curSym->position);

			}
		}
		else Error(14);

		Get();
	}

	//begin
	else if(!strcmp(token, "begin")){
		Get();
		Statement();
		while(!strcmp(token, ";")){
			Get();
			Statement();
		}
		if(!strcmp(token, "end")){
			Get();
		}
		else{

			Error(10);
		}
	}

	//if - then
	else if(!strcmp(token, "if")){

		Get();
		Condition();
		if(!strcmp(token, "then")){
			Get();
			//store the line where our jump address is written for future use
			jaddr = line;
			Gen(8,0,0);
			Statement();

			if(!strcmp(token, ";")){
				//update our jump addresses to the correct one
				codegen[jaddr].m = line;
				Get();
			}

			if(!strcmp(token, "else")){
				jaddr2 = line;
				Gen(7,0,0);
				Get();
				codegen[jaddr].m = line;
				Statement();
				//update our jump addresses to the correct one
				codegen[jaddr2].m = line;
				}
			else{
				codegen[jaddr].m = line;
				token = ";";
				tptr--;
			}


		}
		else Error(16);
	}

	//while
	else if(!strcmp(token, "while")){

		Get();
		//store the position of the start of the loop
		caddr = line;
		Condition();
		if(!strcmp(token, "do")){
			//store the line where our jump address is written for future use
			jaddr = line;
			Gen(8,0,0);
			Get();
			Statement();
			//write the jump to the top of the loop
			Gen(7,0,caddr);
			//update our jump address to the correct one
			codegen[jaddr].m = line;
		}
		else Error(18);
	}

	//read from console
	else if(!strcmp(token, "read")){

		Gen(10,0,0);
		Get();

		//ident to store input
		if(!strcmp(token, "2")){

			//actual variable
			ident = input[tptr++];
			Get();

			//get the current symbol and make sure it is a variable
			curSym = Lookup(st, ident);
			if(curSym == NULL){
				Error(11);
			}
			else if(curSym->type != 1){
				Error(12);
			}

			//if the variable has not yet been assigned,
			//assign its position on the stack
			if(curSym->position == -1){
				curSym->position = offset++;
			}
			Gen(4,(lex - curSym->level),curSym->position);


		}
	}

	//write to console
	else if(!strcmp(token, "write")){

		Get();

		if(!strcmp(token, "2")){

			ident = input[tptr++];
			Get();

			//get the current symbol and make sure it is a variable
			curSym = Lookup(st, ident);
			if(curSym == NULL){
				Error(11);
			}
			else if(curSym->type != 1){
				Error(12);
			}

			if(curSym->position == -1){
				Error(11);
			}

			Gen(3,(lex - curSym->level),curSym->position);
			Gen(9,0,0);
		}
	}
}


void PList(){

	int parms = 0;
	int i;

	Get();
	if(!strcmp(token, "(")){
		Get();
		if(strcmp(token, ")")){
			Expression();
			parms++;
			while(!strcmp(token, ",")){
				Get();
				Expression();
				parms++;
			}

			while(parms > 0){
				Gen(4, 0, stacksize + 4-1);
				parms--;
			}

			if(strcmp(token, ")")){
				Error(22);
			}
		}
	}
	else Error(22);
}




int PBlock(){

	int tmp;
	int vars = 0;

	char* ident = malloc(sizeof(char*) * 12);

	if(!strcmp(token, "(")){
		Get();
		if(!strcmp(token, ")")){
			Get();
			return vars;
		}
		if(!strcmp(token, "2")){

			ident = input[tptr++];
			tmp = Add(st, ident, 1, 0, offset++, lex+1);
			if(tmp == 0) vars++;
			else printf("Error inserting into table. Code - %d\n", tmp);

			Get();

			while(!strcmp(token, ",")){
				Get();
				if(!strcmp(token, "2")){
					ident = input[tptr++];
					tmp = Add(st, ident, 1, 0, offset++, lex+1);
					if(tmp == 0) vars++;
					else printf("Error inserting into table. Code - %d\n", tmp);
					Get();

				}
				else Error(19);
			}
			if(!strcmp(token, ")")){
				Get();
			}
			else{
				Error(22);
			}

		}
		else {
			Error(16);
		}
	}
	else{
		Error(22);
	}

	return vars;
}

//parse a block
char* Block(int v){

	lex++;

	int vars = v;
	char* ident = malloc(sizeof(char*) * 12);
	int number;
	int tmp;
	int jaddr;
	int bgn;
	int procvars;

	//store our constants
	if(!strcmp(token, "const")){

		do{
			Get();
			if(!strcmp(token, "2")){
				ident = input[tptr++];
			}
			else{
				Error(4);
			}

			Get();

			if(!strcmp(token, "=")){
				Get();
				if(!strcmp(token, "3")) number = atoi(input[tptr++]);
				else Error(2);
			}
			else{
				Error(3);
			}

			tmp = Add(st, ident, 0, number, -1, lex);
			if(tmp == 0);
			else printf("Error inserting %d into table. Code - %d\n",number, tmp);
			Get();

		} while(!strcmp(token, ","));

		if(!strcmp(token, ";")) Get();
		else {
			Error(10);
		}
	}


	//store our variables
	if(!strcmp(token, "var")){

		do{
			Get();
			if(!strcmp(token, "2")){
				ident = input[tptr++];
			}
			else{
				Error(4);
			}
			tmp = Add(st, ident, 1, number, offset++, lex);
			if(tmp == 0);
			else printf("Error inserting %d into table. Code - %d\n",number, tmp);
			Get();
			vars++;
		}while(!strcmp(token, ","));


		if(!strcmp(token, ";")) Get();
		else {
			Error(10);
		}
	}

	//allocate our stack space for our constants and variables
	bgn = line;
	Gen(7, 0, line);



	//parse all procedures
	while(!strcmp(token, "procedure")){
		procvars = 0;
		offset = 4;
		Get();
		if(!strcmp(token, "2")){
			ident = input[tptr++];
			Get();
		}
		else{
			Error(4);
		}

		procvars = PBlock();


		if(!strcmp(token, ";")) Get();
		else Error(6);

		Add(st, ident, 2, 0, line, lex);
		Add(st, "return", 1, 0, 0, lex+1);

		Block(procvars);
		if(!strcmp(token, ";")) Get();
		else{
			Error(10);
		}
		Gen(2,0,0);
	}

	codegen[bgn].m = line;
	Gen(6, 0, vars + 4);
	Statement();
	Delete(st);
	lex--;
	Gen(11, 0, 3);
	return token;
}


int main(void){
 	Initialize2("lexicalOutput.txt", "mcode.txt");
	Get();
 	Block(0);
	if(!strcmp(token, ".")){
 		printOut();
		fclose(fp);
 	}
 	else Error(9);

 	return 0;
 }