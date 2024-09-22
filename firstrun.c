#include "project_defines.h"


//This function will execute the first pass of the assembler and look for symbols and errors

bool firstrun (FILE *newfp, label **labelHead, int *ICmain, int *DCmain){

    int  lineCounter = ZERO, code, i, j, dataNum = ZERO, IC = ZERO, DC = ZERO, val, commas = ZERO;
    char line[MAX_LINE_LENGTH], *labelWithoutColon;            /* Will contain the current line*/
    int wordCounter = ZERO;           /* Amount of words in the line*/
    char *wordArray[MAX_LINE_LENGTH];        /* Will contain an array of the line divided into words*/
    bool errorFound = false, labelFound = true, extFound = true, defineFound = false;
    label *labelCurr = NULL;



while (fgets (line, MAX_LINE_LENGTH, newfp)){ 			//read new line from file
	wordCounter = makeItArray(line, wordArray);		//Separate the line into words separated by spaces and insert them
 if (wordArray[ZERO][ZERO] != ';' || strcmp(wordArray[ZERO], "\n") != ZERO || strcmp(wordArray[ZERO], "\t") == ZERO){//Check if it's an empty or a comment line 
	lineCounter++;
	j = ZERO;
	labelCurr = *labelHead;
	labelFound = true;


	if ((wordArray[j][strlen(wordArray[j]) - ONE]) ==':'){  //The last char of the first word is ':' means is a label
		labelWithoutColon = removeColon(wordArray[j]);
		labelFound = checkLabel(labelCurr, labelWithoutColon);
		if (labelFound == false){
			fprintf(stderr,"Error in line %d. Label error\n", lineCounter);
			errorFound = true;
			}
		else if (strcmp(wordArray[j + ONE], ".extern") == ZERO){
			fprintf(stderr,"Warning in line %d. Label error; extern is defined with after label. Assembler will ignore label\n", lineCounter);
			j = TWO;
			}
		else {
			labelCurr = build_label ();
			strcpy(labelCurr->Name, labelWithoutColon);			//Inserting the label's name into the node
			labelCurr->next = *labelHead;
			labelCurr->symbol = symcode;
			labelCurr->value = IC + ADDRESS_START;
			labelCurr->counter = ZERO;
			j = ONE;	
			}
		}

	if (strcmp(wordArray[j + ONE], ":") == ZERO){//look for space before semi colon error
			fprintf(stderr,"Error in line %d. Label error; space before :\n", lineCounter);
			errorFound = true;
			}

       	if ((strcmp(wordArray[j], ".string") == ZERO)){ //check if word is .string 
		if (wordCounter > j + TWO){
			fprintf(stderr,"Error in line %d. Too many arguments for data or string command \n", lineCounter);
			errorFound = true;
			}
		labelCurr->value = DC + IC + ADDRESS_START;
		labelCurr->symbol = data;
		dataNum = countLetters(wordArray[j + ONE]);				
		DC+= dataNum;	
	}
	else if (strcmp(wordArray[j], ".data") == ZERO){ //check if word is .data 
		labelCurr->value = DC + IC + ADDRESS_START;
		labelCurr->symbol = data;
		dataNum = ZERO;
		commas = ZERO;
		for (i = j; i < wordCounter; i++){//Scan for commas
  			for (int k = ZERO; k < strlen(wordArray[i]); k++) {
      			 	 if (wordArray[i][k] == ',') 
          		 	 commas++;
			}
		}
		dataNum = commas + ONE; // add the amount of words
		if (wordCounter - TWO  != commas + ONE)
			dataNum = MINUS_ONE;

		if (dataNum == MINUS_ONE){
			fprintf(stderr,"Error in line %d. Commas are not coherent with data\n", lineCounter);
			errorFound = true;
			}
		else
			DC+= dataNum;
		}
	else if (strcmp(wordArray[j], ".define") == ZERO) { //new define found
		if (wordCounter > j + FOUR){
			fprintf(stderr,"Error in line %d. Too many arguments for define definition \n", lineCounter);
			errorFound = true;//need to check if define can be a formula like macro
			break;
			}
		defineFound = checkLabel(labelCurr, wordArray[j + ONE]);
		if (defineFound == false){
			fprintf(stderr,"define error in line %d. define already defined as label\n", lineCounter);
			errorFound = true;
			}
		labelCurr = build_label();
		strcpy(labelCurr->Name, wordArray[j + ONE]);
		labelCurr->next = *labelHead;
		labelCurr->symbol = mdefine;
		val = atoi(wordArray[j + THREE]);
		labelCurr->value = val; 
		labelCurr->lineNum = lineCounter; 
		*labelHead = labelCurr;
		}
	else if (strcmp(wordArray[j], ".extern") == ZERO){//new extern found
		extFound = checkLabel(labelCurr, wordArray[j + ONE]);
		if (extFound == false){
			fprintf(stderr,"Error in line %d. Extern is already defined\n", lineCounter);
			errorFound = true;
			}
		labelCurr = build_label();
		labelCurr->symbol = external;
		labelCurr->next = *labelHead;
		labelCurr->counter = ZERO;
		for (i = j + ONE; i < j + wordCounter; i++)
			strcat(labelCurr->Name,wordArray[i]);
		*labelHead = labelCurr;
	}	
	else {  //just a command 
		IC++;
		code = CalcL(wordArray[j]);
		if (code == TWO){ //command receives 2 operands - need to check if one or two additional words
			if (wordCounter != j + THREE){//check if the command received the right amount of operands
				fprintf(stderr,"Error in line %d. Wrong amount of arguments for this operand\n", lineCounter);
				errorFound = true;
				}
			if(checkOpernad(wordArray[j + ONE], labelCurr) == THREE && checkOpernad(wordArray[j + TWO], labelCurr) == THREE) 
				code = ONE;
			}
		else if (code == ONE){ //command receives 1 operands
			if (wordCounter!= j + TWO){
				fprintf(stderr,"Error in line %d. Wrong amount of arguments for this operand\n", lineCounter);
				errorFound = true;
				}
			}
		else if (code == ZERO){ //command receives 0 operands
			if (wordCounter != j + ONE){
				fprintf(stderr,"Error in line %d. Wrong amount of arguments for this operand\n", lineCounter);
				errorFound = true;
				}
			}
		IC += code;		
		}
	for (i = j; i < wordCounter; i++){
			 if(strchr(wordArray[i], '[') != NULL && strchr(wordArray[i], ']') != NULL)
				IC++;			
			 else if(strchr(wordArray[i], '[') != NULL && strchr(wordArray[i], ']') == NULL){
				fprintf(stderr,"Error in line %d. Square brackets opened but not closed\n", lineCounter);
				errorFound = true;
				}	
			else if(strchr(wordArray[i], '[') == NULL && strchr(wordArray[i], ']') != NULL){
				fprintf(stderr,"Error in line %d. Square brackets closed but not opened\n", lineCounter);
				errorFound = true;
				}
			}
*labelHead = labelCurr;
	}
}

if (errorFound == false){
	*ICmain = IC;
	*DCmain = DC;
	rewind(newfp);
	return true;
	}
else
	return false;
}

  
