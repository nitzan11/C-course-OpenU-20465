#include "project_defines.h"

static char *savedWords[] = {"mov","cmp","add","sub","not","clr","lea","inc","dec","jmp","bne","red","prn","jsr","rts","hlt","r0","r1","r2","r3","r4","r5","r6","r7",".data",".string",".entry",".extern", ".define"}; 

//Sentence comprehension fucntions 
int makeItArray(char line[MAX_LINE_LENGTH], char **wordArray) {//This function divides the sentence into an array that contains the words of the sentence
    int wordIndex = ZERO;
    char *token; // token for taking each word

    token = strtok(line, " \n\t");
    while (token != NULL) {
        wordArray[wordIndex] = token;
        wordIndex++;
        token = strtok(NULL, " \n\t");
    }
    return wordIndex;
}

char *removeColon(char *labelBefore) {
    char *token;
    token = strtok(labelBefore, ":");
    return token;
}

int checkOpcode (char word []){
for (int i = ZERO; i < AMOUNT_OF_COMMANDS; i++){ //compare word to saved words
		if (strcmp(word,savedWords[i]) == ZERO)
			return i;
	}	
return MINUS_ONE;
}


int checkOpernad (char *word, label *curr){//this function is characterizing the operand
int val = word[ONE] - '0';
char *token;

token = strtok(word, ",");

if (token[ZERO] == 'r' && val <= SEVEN && val >= ZERO) //check if word is a register and return three
	return THREE;

if (token[ZERO] == '#' && token[ONE] == '-') //check if word is a number and return zero
	return ZERO;
else if (token[ZERO] == '#' && token[ONE] == '+')  // Check for a positive number
	return ZERO;
else if (token[ZERO] == '#' && isdigit(token[ONE]) != ZERO)
	return ZERO;
else if (token[ZERO] == '#')
	return ZERO;


while (curr){		//scan through the list
	if (strstr(token,curr->Name) != NULL  && curr->symbol == mdefine){//check if word is a define and return one
		return ZERO;
		}
	else if (strcmp(token,curr->Name) == ZERO && curr->symbol == external)//check if word is an external label and return four
		return FOUR;
	else if (strcmp(token,curr->Name) == ZERO)//check if word is a label and return one
		return ONE;
	else if (strstr(token,curr->Name) != NULL && strchr(token, '[') != NULL)//check if word is a label wih index and return two
		return TWO;
	else 
		curr = curr->next;
		}

return MINUS_ONE;
}

int CheckRegNum (char *word){
int val = word[ONE] - '0';
if (word[ZERO] == 'r' && val <= SEVEN && val >= ZERO) //check if word is a register 
	return val;

return MINUS_ONE;
}

int countLetters(char *word){
   int letters = ZERO, i;
    for (i = ZERO; i < strlen(word); i++){
	if (isalpha(word[i]) != ZERO) 
        	letters++;
		}
    if(word[ZERO] != '"')
    	fprintf(stderr,"Error. String not starting with correct string marker\n");
    if(word[letters + ONE] != '"')
	 fprintf(stderr,"Error. String not ending with correct string marker\n");
return letters + ONE;
}

int checkNum(char *word, label *curr) {
    int sign = ONE;
    label *temp = curr;
    int val;
    char *start = strchr(word, '['), *end = strchr(word, ']'), numberStr[TEN];


    if (word[ZERO] == '#' && word[ONE] == '-') { // Check for a negative number
        sign = MINUS_ONE;
        val = (word[TWO] - '0') * sign; // Convert the character digit to its integer value and apply the sign
        return val;
    } 
    else if (word[ZERO] == '#' && word[ONE] == '+') { // Check for a positive number
        val = (word[TWO] - '0') * sign; // Convert the character digit to its integer value and apply the sign
        return val;
    }
    else if (word[ZERO] == '#' && word[ONE] == '+') { // Check for a positive number
        val = (word[ONE] - '0') * sign; // Convert the character digit to its integer value and apply the sign
        return val;
    }
    else if (word[ZERO] == '#' && isdigit(word[ONE]) != ZERO) { // Check for a positive number
        val = (word[ONE] - '0') * sign; // Convert the character digit to its integer value and apply the sign
        return val;
    }
    else if (word[ZERO] == '#'){//scan defines if starting with #
	while (curr){		//scan through the list
		if (strstr(word,curr->Name) != NULL  && curr->symbol == mdefine)//define found in line
			return  curr->value;
		else 
			curr = curr->next;
		}
	}

if (start != NULL){
	while (temp){		//scan through the list
		if (strstr(word,temp->Name) != NULL  && temp->symbol == mdefine)//define found in line
			return  temp->value;
		else 
			temp = temp->next;
		}

	size_t length = end - start - ONE; //calculate number's length 
	strncpy(numberStr, start + ONE, length);
	numberStr[length] = '\0';
	int result = atoi(numberStr);
	return result;
	}

    return ZERO; // Return 0 by default if the word doesn't meet the condition
}

int checkLabelAddress(char *word, label *curr){
	while (curr){ 		//scan through the list
		if (strcmp(word,curr->Name) == ZERO)//label found
			return curr->value;
		else if (strstr(word,curr->Name) != NULL  && strchr(word, '[') != NULL)
			return curr->value;
		else 
			curr = curr->next;
		}

return ZERO;
}


int CalcL(char *word){
	if ((strcmp(word,"mov") == ZERO) || (strcmp(word,"cmp") == ZERO) || (strcmp(word,"add") == ZERO) || (strcmp(word,"sub") == ZERO) || (strcmp(word,"lea") == ZERO))
		return TWO;
	else if ((strcmp(word,"not") == ZERO) || (strcmp(word,"clr") == ZERO) || (strcmp(word,"inc") == ZERO) || (strcmp(word,"dec") == ZERO) || (strcmp(word,"jmp") == ZERO) || (strcmp(word,"bne") == ZERO) || (strcmp(word,"red") == ZERO) || (strcmp(word,"prn") == ZERO) || (strcmp(word,"jsr") == ZERO))
		return ONE;
	else if ((strcmp(word,"rts") == ZERO) || (strcmp(word,"hlt") == ZERO))
		return ZERO;
	else 
		return MINUS_ONE;
}

void printBinary(int num, int ic) {
    int i;
    unsigned int mask = 1 << (sizeof(short int) * 8 - 1); // Assuming 32-bit integers
    printf("Binary representation of %d in line %d is: ",num, ic + 100);

    for (i = 0; i < sizeof(short int) * 8; i++) {
        printf("%d", (num & mask) ? 1 : 0);
        num <<= 1; // Shift the bits to the left
        if ((i + 1) % 4 == 0) { // Add space after every 4 bits for better readability
        }
    }
    printf("\n");
}

//Name check functions for nodes
bool checkMacro(int n, char word []){
	if (n > TWO){
		fprintf(stderr,"Error. Macro name too long\n");
		return false;
		}
	for (int i = ZERO; i < SAVED_WORDS; i++){ //check macro's name is not a command
		if (strcmp(word,savedWords[i]) == ZERO){//macro name is illegal
			fprintf(stderr,"Error. Macro name can't use command %s name\n", savedWords[i]);
			return false;
		}
	}
return true; //macro name is ok
}


bool checkLabel (label *curr, char *word){
	while (curr){ 						//scan through the labels list
		if (strcmp(word,curr->Name) == ZERO){		//label's name exists in label table
			fprintf(stderr,"Error. Label already exists\n");
			return false;
			}
		else if (isalpha(word[ZERO]) == ZERO){		//first character can't be a number
			fprintf(stderr,"Error. Label can't start with a number\n");
			return false;
			}
		else
			curr = curr->next;
	}
	for (int i = ZERO; i < SAVED_WORDS; i++){ //check label's name is not a saved  word
		if (strcmp(word,savedWords[i]) == ZERO){//labels name is illegal
			fprintf(stderr,"Error. label was used as a saved word %s\n", savedWords[i]);
			return false;
		}
	}
	if (strlen(word) >= TWO && isspace(word[strlen(word) - TWO]) != ZERO) {//need to fix while breaking words into their respective parts
   		 fprintf(stderr, "Error. White space before :\n");
  		  return false;
		}
	if (strlen(word) >  MAX_LABEL_SIZE){//check label's length
		fprintf(stderr,"Error. Label's name too long\n");
		return false;		
	}
	
return true;
}


//free_data functions -receives the head of the data list and free memory
void free_data(macro *data_head)
{
    macro *temp;
    while (data_head)
    {	
	free(data_head->Name);
	free(data_head->Content);
        temp = data_head->next;
        free(data_head);
        data_head = temp;
    }
}

void free_label(label **data_head) {
    label *temp;
    while (*data_head) {
        temp = (*data_head)->next;
        free(*data_head);
        *data_head = temp;
    }
}


//Node building functions
macro *build_node (int nameSize, int contSize){
macro *new;
	new = (macro*)malloc(sizeof(macro));//Dynamically allocate macro node
        if (new == NULL) {
        	printf("Error. Failed to allocate memory for node\n");
               	exit(1);
               	}
	new->Name = (char*)realloc(new->Name,nameSize);//Dynamically allocate macro node name & content
        if (new->Name == NULL) {
               	printf("Error. Failed to allocate memory for name\n");
               	exit(1);
               	}
	new->Content = (char*)realloc(new->Content,contSize);
        if (new->Content == NULL) {
               	printf("Error. Failed to allocate memory for content\n");
               	exit(1);
               	}
return new;
}

label *build_label (void){
label *new;
	new = (label*)malloc(sizeof(label));//Dynamically allocate label node
        if (new == NULL) {
        	printf("Error. Failed to allocate memory for node\n");
               	exit(1);
               	}
return new;
}

//Files creation functions
void createObjectFile(FILE *objectFile, int DC, int IC, short int *data, short int *comm) {
int i = ZERO;
char* encrypted;
fprintf(objectFile, "%d %d\n", IC, DC); 
for (i = ZERO; i < IC; i++){
	encrypted = encrypt(comm[i]);
        if (encrypted != NULL) {
		if (i < ONE_THOUSAND){
           		//printf("%d%d %.7s\n", ZERO, i + ADDRESS_START, encrypted); 
	   		fprintf(objectFile, "%d%d %.7s\n",ZERO, i + ADDRESS_START, encrypted); 
		}
		else{
           		//printf("%d %.7s\n", i + ADDRESS_START, encrypted); 
	   		fprintf(objectFile, "%d %.7s\n", i + ADDRESS_START, encrypted); 
		}
        } 
	else {
            printf("Memory allocation failed.\n");
            break;  // Exit the loop if memory allocation fails
       	 }
}

for (i = ZERO; i < DC; i++){
        encrypted = encrypt(data[i]);
        if (encrypted != NULL) {
		if (i < ONE_THOUSAND){
           		//printf("%d%d %.7s\n", ZERO, i + ADDRESS_START + IC, encrypted); 
	   		fprintf(objectFile, "%d%d %.7s\n",ZERO, i + ADDRESS_START + IC, encrypted); 
		}
		else{
           		//printf("%d %.7s\n", i + ADDRESS_START + IC, encrypted); 
	   		fprintf(objectFile, "%d %.7s\n", i + ADDRESS_START + IC, encrypted); 
		}
	}
	else {
            printf("Memory allocation failed.\n");
            break;  //Exit the loop if memory allocation fails
        }
  }
free(encrypted); 
}

char* encrypt(short int num) {
    char* base4 = (char*)malloc(SEVEN * sizeof(char));  //Dynamically allocate memory
    if (base4 == NULL) {
        return NULL;
    }
    
    base4[SIX] = '\0';  //Null-terminate the string
    int index = SIX, inverted_number;

    if (num < ZERO){ //Convert negative to positive 
	inverted_number = ~num;
	num = ++inverted_number;
	}
    while (index >= ZERO) {
        short int remainder = num % FOUR;
        switch (remainder) {
            case 0: base4[index--] = '*'; break;
            case 1: base4[index--] = '#'; break;
            case 2: base4[index--] = '%'; break;
            case 3: base4[index--] = '!'; break;
        }
        num /= FOUR;
    }

    //Fill any remaining positions with '*' if needed
    while (index >= ZERO) {
        base4[index--] = '*';
    }

    return base4;  //Return the dynamically allocated pointer
}

void createExtFile(FILE *extFile, label *curr) {
    while (curr) {
        for (int i = ZERO; i < curr->counter; i++) {
            fprintf(extFile, "%s %d\n", curr->Name, curr->addresses[i]);
        }
        curr = curr->next;
    }
}

void createEntFile(FILE *entFile, label *curr) {
    while (curr) {
	if(curr->symbol == entry)
        	fprintf(entFile, "%s %d\n", curr->Name, curr->value);
        curr = curr->next;
    }
}


