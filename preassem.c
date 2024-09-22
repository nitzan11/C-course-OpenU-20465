#include "project_defines.h"


//This function will receive a file and deploy the macros into a new file

bool deployMacro(FILE *fp, FILE *amfp){ 
bool macrosFound = false;
int ContentSize = ZERO, i = ZERO, wordCounter = ZERO;
char line[MAX_LINE_LENGTH];
char *wordArray[MAX_LINE_LENGTH], *name = NULL;        //contains an array of the line divided into words
macro *curr, *head = NULL;
//printf("Received files\n");
while (fgets (line, MAX_LINE_LENGTH, fp)){ 		//read new line from file
	curr = head;	
	wordCounter = makeItArray(line, wordArray);//Separate the line into words separated by spaces 
	while (curr){ //scan through the macro list
		if (strcmp(wordArray[ZERO],curr->Name) == ZERO){//macro name exists in macro table
			fprintf(amfp, "%s", curr->Content);// print words to file if macro is already
			if (fgets (line, MAX_LINE_LENGTH, fp)){
				wordCounter = makeItArray(line, wordArray);
				break;
				}
			}
		else
			curr = curr->next;
	}

	if (strcmp(wordArray[ZERO], "mcr") == ZERO){ 				//found mcro	
		//printf("found macro\n");	
        	macrosFound = checkMacro(wordCounter, wordArray[ONE]); //new macro found           
		//printf("macro found is %d\n", macrosFound);  
        	if (macrosFound == true){ 					//create new node
			name = (char*)realloc(name,strlen(wordArray[ONE]));
      			if (name == NULL) {
               			printf("Error. Failed to allocate memory for content\n");
               			exit(1);
               			}			
			strcpy(name, wordArray[ONE]);
			if (fgets(line, MAX_LINE_LENGTH, fp) == NULL)  //Read macro content	
				break;
        		wordCounter = makeItArray(line, wordArray); 		//Separate the line into
			ContentSize = ZERO;
			for (i = ZERO; i < wordCounter; i++)
				ContentSize += strlen(wordArray[i]) + ONE;
			curr = build_node (strlen(name) + ONE, ContentSize);
                	strcpy(curr->Name, name);	//Inserting the macro's name into the node
			curr->next = head;
			head = curr;
			for (i = ZERO; i < wordCounter; i++){
				strcat(head->Content, wordArray[i]);
                      		strcat(head->Content, " ");
				}
                      	strcat(head->Content, "\n");
			while (fgets (line, MAX_LINE_LENGTH, fp)){//if first word is not endcmcro then it's a macro content that we need to save content's into the node
        			wordCounter = makeItArray(line, wordArray);//Separate the line 
				if (strcmp(wordArray[ZERO], "endmcr") == ZERO){
					if (wordCounter > ONE){
						printf("Error. Too many arguments in endmcr definition\n");
               					exit(1);
						}
					else
						break;
				}
				else {
					for (i = ZERO; i < wordCounter; i++)
						ContentSize += strlen(wordArray[i]) + ONE;
					head->Content = (char*)realloc(head->Content,ContentSize);
      					if (head->Content == NULL){
               					printf("Error. Failed to allocate memory for content\n");
               					exit(1);
               					}
					for (i = ZERO; i < wordCounter; i++){
						strcat(head->Content, wordArray[i]);
                      				strcat(head->Content, " ");
						}
                      			strcat(head->Content, "\n");
					}
				}
			macrosFound = false;
			}
		}
	else{					//newline is not a macro
		for (i = ZERO; i < wordCounter; i++)
			fprintf(amfp, "%s ", wordArray[i]);//print words to file in case not a macro
		fprintf(amfp, "\n");			//print words to file in
		}
	}
    free_data(head);
    //printf("Memory released successfully\n");
    rewind(amfp);
    return true;
}


