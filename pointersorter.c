#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*
	Defines a struct that contains all the variables required to 
	store and handle an array of tokens. 
	
	index: tracks current index of tokens array for the getTokens function
	tokensLength: stores the current length of tokens array
	input: points to a copy of the main function arguments
	tokens: points to a dynamically allocated array of tokens parsed from input
*/	
struct Tokenizer_ {
	int index;		//tracks current index for getTokens function
	int tokensLength;	//stores the len
	char *input;
	char **tokens;
};

typedef struct Tokenizer_ Tokenizer;

/*
	Implements selection sort to sort the tokens in ascending alphabetical 
	order according to the ASCII ordering.
	Ex. A a B b C c would sort to A B C a b c
	
	tk: points to a tokenizer
	
	returns: nothing

*/
void sortTokens(Tokenizer *tk){
	int sorted = 0;
	char* min;
	int i;
	while (sorted != tk->tokensLength){
		min = tk->tokens[sorted];
		for(i = sorted + 1; i < tk->tokensLength; i++){
			if (strcmp(tk->tokens[i], min) < 0){
				min = tk->tokens[i];
			}
		}
		if (min != tk->tokens[sorted]){
			char* temp = (char*)malloc((strlen(min) + 1) * sizeof(char));
			strcpy(temp, min);
			strcpy(min, tk->tokens[sorted]);
			strcpy(tk->tokens[sorted], temp);
			free(temp);	
		}
		sorted++;
	}
}

/*
	Increments the index to point to the next unreturned token. For use in 
	traversing the tokens array.
	
	tk: points to a tokenizer
	
	returns: next token in tokens array
*/
char *getToken(Tokenizer *tk){
	
	if (tk->index == tk->tokensLength){
		return NULL;
	}
	char *token = tk->tokens[tk->index];
	tk->index++;
	return token;
}

/*
	Appends a char to a string.
	
	s: string that will be appended to
	c: char that will be appended to s
	
	returns: the new string
*/
char* append(char* s, char c){
	//printf("entered funtion:append\n");
	if (s == NULL){
		s = (char*)malloc(sizeof(char));
		s[0] = '\0';
	}
	int len;
	char *stemp;
	
	len = strlen(s);
	stemp = (char*)realloc(s, len + sizeof(char));
	if (stemp == NULL){
		printf("realloc in function:append failed\n");
		free(s);
		return NULL;
	}
	s = stemp;
	stemp = NULL;
	free(stemp);
	s[len] = c;
	s[len + 1] = '\0';
	return s;
	
}

/*
	Appends a token to the end of the tokens array and increments tokensLength
	
	tk: points to a tokenizer
	token: token to be appended
	index: for inserting token into newly allocated spot in tokens array
	
	returns: nothing
*/
void appendToken(Tokenizer *tk, char *token, int index){
	char **temp;
	tk->tokensLength++;
	temp = realloc(tk->tokens, (tk->tokensLength * sizeof(char*)) + sizeof(char*));
	if (temp == NULL){
		printf("realloc in function:tokenize failed\n");
		free(temp);
		return;
	}
	tk->tokens = temp;
	tk->tokens[index] = token;
	temp = NULL;
} 

/*
	Breaks a string into an array of tokens. String is split using
	all english non-alphabetical characters as delimiters.
	
	tk: points to a tokenizer
	string: the string which is to be tokenized
	
	returns: nothing
*/
void tokenize(Tokenizer *tk, char *string){
	
	if (string == NULL){
		return;
	}
	int length = strlen(string);
	if (length == 0){
		return;
	}
	char *token;
	token = NULL;
	int index = 0;
	int processingToken = 1;
	int i;
	for (i = 0; i < length; i++){
		if (isalpha(string[i])){
			token = append(token, string[i]);
			processingToken = 1;
		}
		else{
			processingToken = 0;
			if (token != NULL){
				appendToken(tk, token, index);	
				index++;
			}
			token = NULL;	
		}
		
	}
	//this appends the final token if there is no delimiter following
	if (processingToken) {
		appendToken(tk, token, index);
		token = NULL;
	}
	
}

/*
	Initializes a tokenizer and makes a copy of the command line
	argument which is pointed to by input. Also tokenizes the 
	input and fills an array with tokens.
	
	returns: an initialized tokenizer if no error
	returns: NULL if input is NULL or copying fails
	
	input: the command line argument
*/
Tokenizer *createTokenizer(char * input) {
	if (input == NULL){
		return NULL;
	}
	
	Tokenizer *tk = (Tokenizer *)malloc(sizeof(Tokenizer));
	char *copy;
	copy = (char*)malloc(strlen(input) + 1);
	
	if (copy == NULL){
		return NULL;
	}
	
	strcpy(copy, input);
	tk->input = copy; 
	tk->tokensLength = 0;
	tk->index = 0;
	tokenize(tk, tk->input); 
	
  	return tk;
}

/*
	Frees all dynamically allocated memory from the tokenizer 
	initialization.
	
	tk: points to a tokenizer
	
	returns: nothing
*/
void freeTokenizer(Tokenizer  *tk){
	int i;
	for (i = 0; i < tk->tokensLength; i++){
		free(tk->tokens[i]);
	}
	free(tk->tokens);
	free(tk->input);
}

/*
	Takes a single string as an argument from argv[1], breaks the string
	into tokens using all non alphabetical characters as delimiters and prints
	the tokens in ascending alphabetical order. 
	
	argc: length of argv
	argv: command line arguments
	
	returns: 0
*/
int main(int argc, char **argv){

	if (argc == 1){
		printf("\nToo few arguments. Correct usage is: \n\n./pointersorter \"some string\"\n\n");
		return 0;
	}
	else if (argc > 2){
		printf("\nToo many arguments. Correct usage is: \n\n./pointersorter \"some string\"\n\n");
		return 0;
	}
	
	Tokenizer *tk = createTokenizer(argv[1]);
	if (tk != NULL) {
		sortTokens(tk);
		int i;
		for (i = 0; i < tk->tokensLength; i++){
			char * token = getToken(tk);
			if (token != NULL){
				printf("%s\n", token);
			}
		}
		freeTokenizer(tk);
	}	
	return 0;
}
