#include <stdio.h>
#include <stdlib.h>
#define ZERO 0
#define ONE 1
#define TEN 10
#define TWENTY_ONE 21
#define THIRTY 30 


int my_strcasecmp(char *stringA, char *stringB)
{
	while(*stringA!='\0' && (*stringA>='a'?*stringA:*stringA+'a'-'A')==(*stringB>='a'?*stringB:*stringB+'a'-'A') )
	{
		stringA++;
		stringB++;	
	}
	return (*stringA>='a'?*stringA:*stringA+'a'-'A')-(*stringB>='a'?*stringB:*stringB+'a'-'A');
}

/*this program will generate names randomly from a global list of names*/
char names [THIRTY][TWENTY_ONE];
char* get_name();

int main ()
{
srand(time(ZERO));
int i, j = ZERO;
printf("Please enter the names required for this assignment:\n");
printf("Reading names...\n");
while (scanf(" %s", names[j]) == ONE){
	for (i = ZERO; i < j; i++)
		if (my_strcasecmp(names[i],names[j]) == ZERO){
		printf("Error was found. Aborting program \n");
		exit(ZERO);
		}	
	j++;
	}
printf("Printing names:\n");
for (i = ZERO; i < THIRTY; i++)
	printf("%s\t", names[i]);

printf("\ngenerating random names:\n");
for (i = ZERO; i < TEN; i++){
	printf("%s\n", get_name());
	}
printf("Program finished successfully\n");
}


char* get_name(){
int randNum;
randNum = rand() % THIRTY;
return names[randNum];

}


