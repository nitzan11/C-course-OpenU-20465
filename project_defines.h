
//Program libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



//Defines that will be used in the Assembler program 
#define MINUS_TWO -2
#define MINUS_ONE -1
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 9
#define TEN 10
#define ELEVEN 11
#define TWELVE 12
#define THIRTEEN 13
#define FOURTEEN 14
#define FIFTEEN 15
#define SIXTEEN 16
#define ACTIVE_BITS 63
#define ADDRESS_START 100
#define ONE_THOUSAND 1000
#define MAX_LINE_LENGTH 81
#define MAX_LABEL_SIZE 32
#define SAVED_WORDS 29
#define AMOUNT_OF_COMMANDS 16
#define ASSEMBLY_FILE_EXTENSION ".as"
#define MACRO_FILE_EXTENSION ".am"
#define ENTRY_FILE_EXTENSION ".ent"
#define EXTERN_FILE_EXTENSION ".ext"
#define OBJECT_FILE_EXTENSION ".ob"
#define EXTERNAL 1
#define RELOCATABLE 2


typedef enum {
	symcode, 
	entry, 
	external, 
	mdefine,
	data
} type;


typedef struct macro {
	char *Name;
	char *Content;
	struct macro* next;
} macro; //linked list node to store a macro

typedef struct label {	
    char Name[MAX_LABEL_SIZE];
    int value;
    type symbol;
    int lineNum;
    int addresses[MAX_LABEL_SIZE];
    int counter;
    struct label *next;
} label; //linked list node to store a label 


//Function prototypes
int makeItArray(char line[MAX_LINE_LENGTH], char **wordArray);

void free_data(macro *data_head);

void free_label(label **data_head);

macro *build_node (int nameSize, int contSize);

bool checkMacro(int n, char word []);

bool deployMacro(FILE *fp, FILE *amfp);

int checkDataOK(char line[MAX_LINE_LENGTH]);

bool firstrun (FILE *newfp, label **labelHead, int *ICmain, int *DCmain);

label *build_label (void);

bool checkLabel (label *curr, char *word);

char *removeColon(char *labelBefore);

int countCommas(char line[MAX_LINE_LENGTH]);

int checkOpcode (char *word);

int checkOpernad (char *word , label *curr);

int checkNum(char *word, label *curr);

void createObjectFile(FILE *objectFile, int DC, int ICF, short int *data, short int *comm);

void secondrun(FILE *fp, char *filename, label **labelHead, int *ICallocation, int *DCallocation);

void createExtFile(FILE *extFile, label *curr);

void createEntFile(FILE *entFile, label *curr);

int checkLabelAddress(char *word, label *curr);

int countLetters(char *word);

int CalcL(char *word);

char* encrypt(short int num);

int CheckRegNum(char *word);

int labelExist(char *target, label *curr);

void printBinary(int num, int ic);
