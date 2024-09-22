#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define ZERO 0
#define ONE 1
#define TWENTY 20
#define MAX_LINE_LENGTH 100 //a default, large enough memory allocation


typedef struct word {	
    char Name[TWENTY];
    int addresses[MAX_LINE_LENGTH];
    int counter;
    struct word *next;
} word; //linked list node to store a word


word *build_word (void){
word *new;
	new = (word*)malloc(sizeof(word));//Dynamically allocate word node
        if (new == NULL) {
        	printf("Error. Failed to allocate memory for node\n");
               	exit(1);
               	}
return new;
}



int compare_words(const char* word1, const char* word2) {
    return strcmp(word1, word2);
}

void insert_sorted(word** head, word* new_word) {
    word* current = *head;
    word* prev = NULL;

    while (current != NULL && compare_words(new_word->Name, current->Name) > ZERO) {
        prev = current;
        current = current->next;
    }

    if (prev == NULL) {
        new_word->next = *head;
        *head = new_word;
    } else {
        prev->next = new_word;
        new_word->next = current;
    }
}

//Function to sort a linked list alphabetically
void sort_alphabetically(word** head) {
    word* sorted = NULL;
    word* current = *head;
    word* next;

    while (current != NULL) {
        next = current->next;
        insert_sorted(&sorted, current);
        current = next;
    }

    *head = sorted;
}
int makeItArray(char line[MAX_LINE_LENGTH], char **wordArray) {//This function divides the sentence into an array that contains the words of the sentence
    int wordIndex = ZERO;
    char *token; // token for taking each word

    token = strtok(line, " \n");
    while (token != NULL) {
        wordArray[wordIndex] = token;
        wordIndex++;
        token = strtok(NULL, " \n");
    }
    return wordIndex;
} 


int main (int argc, char *argv[]){
FILE *fp;
char *filename = NULL, *wordArray[MAX_LINE_LENGTH], line[MAX_LINE_LENGTH];
word *wordHead = NULL, *wordCurr = NULL;
int lineCounter = ZERO, wordCounter, i;
bool wordFound = false;

if (argc == ONE){
	fprintf(stderr, "ERROR: You must enter at least 1 input file name as a command line argument\n");
        exit(2);
   	 }
filename = (char*)realloc(filename,strlen(*++argv));
if (filename == NULL){
	fprintf(stderr,"Failed to allocate memory for filename!\n");
        exit(2);
	}
strcpy(filename,*argv);
if ((fp = fopen(filename, "r")) == NULL){
	fprintf(stderr,"Error. Failed to open file.\n");
	exit(2); 
	}


while (fgets (line, MAX_LINE_LENGTH, fp)){
	wordCounter = makeItArray(line, wordArray);
	lineCounter++;
	wordCurr = wordHead;
	for (i = ZERO; i < wordCounter; i++){
		wordFound = false;
		while (wordCurr){
			if(strcmp(wordArray[i], wordCurr->Name) == ZERO){
				wordCurr->counter++;
				wordCurr->addresses[wordCurr->counter] = lineCounter;
				wordFound = true;
				break;
			}
			else
				wordCurr = wordCurr->next;
			}
		if (wordFound == false){
			wordCurr = build_word ();
			strcpy(wordCurr->Name,wordArray[i]);
			wordCurr->counter = ZERO;
			wordCurr->addresses[wordCurr->counter] = lineCounter;
			wordCurr->next = wordHead;
			wordHead = wordCurr;
			}
		wordCurr = wordHead;
		}
	}
sort_alphabetically(&wordHead);
wordCurr = wordHead;

while (wordCurr){
	printf("%s\t appears in lines ", wordCurr->Name);
	for (i = ZERO; i <= wordCurr->counter; i++){
		if (i == wordCurr->counter)
			printf("%d", wordCurr->addresses[i]);
		else
			printf("%d,", wordCurr->addresses[i]);
		}
	printf("\n");
	wordCurr = wordCurr->next;
	}

wordCurr = wordHead;
while (wordCurr != NULL) {
    word *temp = wordCurr;
    wordCurr = wordCurr->next;
    free(temp);
	}
printf("Memory freed successfully\n");
}
