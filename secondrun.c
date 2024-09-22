#include "project_defines.h"

//This function will execute the second pass of the assembler and build the instructions and data image after all syntax errors were checked in the first run. Therefore we can assume the input is valid except for logical errors, which will be looked after here
void secondrun(FILE *fp, char *filename, label **labelHead, int *ICallocation, int *DCallocation){
	
FILE *fpent, *fpext, *fpobj;
short int *commandTable, *dataTable;
label *labelCurr;
char *wordArray[MAX_LINE_LENGTH], *destfilename = NULL, *dot; 
char line[MAX_LINE_LENGTH];      
int code = ZERO, opcode, i, j, regNum, IC = ZERO, DC = ZERO, destBitsCode, sourceBitsCode, labelAddress, num, wordCounter = ZERO, val, lineCounter = ZERO;
char *token;
bool valFlag = false, errorFound = false, entFlag = false, extFlag = false;
destfilename = (char*)realloc(destfilename,strlen(filename));
	if (destfilename == NULL){
		printf("Failed to allocate memory for filename!\n");
            	exit(2);
		}

commandTable = (short int*)calloc(*ICallocation,sizeof(short int));//Dynamically allocate command table
        if (commandTable == NULL) {
        	printf("Error. Failed to allocate memory for command table\n");
               	exit(1);
               	}
dataTable = (short int*)calloc(*DCallocation, sizeof(short int));//Dynamically allocate data table
        if (dataTable == NULL) {
        	printf("Error. Failed to allocate memory for data table\n");
               	exit(1);
               	}

while (fgets (line, MAX_LINE_LENGTH, fp)){ 		//read new line from file
	wordCounter = makeItArray(line, wordArray);//Separate the line into words separated by spaces 
 if (wordArray[ZERO][ZERO] != ';' || strcmp(wordArray[ZERO], "\n") != ZERO || strcmp(wordArray[ZERO], "\t") == ZERO){//Check if it's an empty or a comment line 	
	labelCurr = *labelHead;
	lineCounter++;
	j = ZERO;
	 if ((wordArray[ZERO][strlen(wordArray[ZERO]) - ONE]) ==':'){  //The last char of the first word is ':' means its a label
			j = ONE;
		if (strcmp(wordArray[j], ".extern") == ZERO)
			j = TWO;
		}

		

	if (strcmp(wordArray[j], ".extern") != ZERO){
		for (i = j; i < wordCounter; i++){
			while (labelCurr){   //scan through the labels list
				if (strcmp(wordArray[i],labelCurr->Name) == ZERO && labelCurr->symbol == external){
					extFlag = true;	
					labelCurr->addresses[labelCurr->counter] = IC + i - j + ADDRESS_START;
					labelCurr->counter++;
					break;
					}
				else
					labelCurr = labelCurr->next;
			}
		labelCurr = *labelHead;
		}
	}

	if (strcmp(wordArray[j], ".define") != ZERO){
		for (i = ZERO; i < wordCounter; i++){
			while (labelCurr != NULL){   //scan through the labels list
				if (strstr(wordArray[i],labelCurr->Name) != NULL  && labelCurr->symbol == mdefine){
					if (labelCurr->lineNum >= lineCounter){
						errorFound = true;
						break;
						}
					break;
					}
				else
					labelCurr = labelCurr->next;
			}
		labelCurr = *labelHead;
		}
	}
	
	if(strcmp(wordArray[j], ".data") == ZERO){ ///Data storage 
		for (i = j; i < wordCounter - ONE; i++){
   			token = strtok(wordArray[j + i], ",");
    			while (token != NULL) {
				while (labelCurr){ //scan through the list
					if (strcmp(token,labelCurr->Name) == ZERO){//define found
						if (labelCurr->lineNum >= lineCounter){
							fprintf(stderr,"Error in line %d. define used before definition\n", lineCounter);
							errorFound = true;
							}
						val = labelCurr->value;
						valFlag = true;
						break;
						}
						else 
							labelCurr = labelCurr->next;
					}
				labelCurr = *labelHead;
				if (valFlag == false)	
					val = atoi(token);
				valFlag = false;
				dataTable[DC] = val;
				DC++;
    				token = strtok(NULL, " ,\n\t");
				}
			}
		}
	else if(strcmp(wordArray[j], ".string") == ZERO){//String storage 
		for (i = ONE; i < strlen(wordArray[j + ONE]) - ONE; i++){
			dataTable[DC] = wordArray[j + ONE][i];
			DC++;
  		}
		dataTable[DC] = ZERO;//NULL sign
		DC++;
		labelCurr = labelCurr->next;
	}
	else if (strcmp(wordArray[j], ".entry") == ZERO){//entry definition
		while (labelCurr){
			if (strcmp(wordArray[j + ONE],labelCurr->Name) == ZERO && labelCurr->symbol == external){//check if entry is defined as external
				fprintf(stderr,"label error in line %d. label already defined as external\n", lineCounter);
				errorFound = true;
				}
			else if (strcmp(wordArray[j + ONE],labelCurr->Name) == ZERO){
				labelCurr->symbol = entry;
				entFlag = true;
				break;
			}
			else
				labelCurr = labelCurr->next;
		}
	}			 
	else{		//Instructions storage section
		code = CalcL(wordArray[j]);
		if (code == TWO){ //command receives 2 operands
			opcode = checkOpcode(wordArray[j]);	
			commandTable[IC] |= opcode<<SIX;
			sourceBitsCode = checkOpernad(wordArray[j + ONE], labelCurr);
			destBitsCode = checkOpernad(wordArray[j + TWO], labelCurr);
			if (destBitsCode == FOUR)
				commandTable[IC] |= ONE<<TWO;
			else
				commandTable[IC] |= destBitsCode<<TWO;
			commandTable[IC] |= sourceBitsCode<<FOUR;
			IC++;
			if (sourceBitsCode == THREE && destBitsCode == THREE){//meaning both are registers
				regNum = CheckRegNum(wordArray[j + ONE]);
				commandTable[IC] |= regNum<<FIVE;
				regNum = CheckRegNum(wordArray[j + TWO]);
				commandTable[IC] |= regNum<<TWO;
				IC++;
				}
			else if (sourceBitsCode == THREE && (destBitsCode == ONE || destBitsCode == FOUR || destBitsCode == TWO)){//source is a register and destination is a label
				regNum = checkOpernad(wordArray[j + ONE], labelCurr);
				commandTable[IC] |= regNum<<FIVE;
				IC++;
				if (destBitsCode == FOUR){//operand is an external label
					commandTable[IC] |= EXTERNAL;
					IC++;
					}
				else{
					labelAddress = checkLabelAddress(wordArray[j + TWO], labelCurr);//else it's a label
					commandTable[IC] |= RELOCATABLE;
					commandTable[IC] |= labelAddress<<TWO;
					IC++;
					}

				if (destBitsCode == TWO){
					num = checkNum(wordArray[j + TWO], labelCurr);
					commandTable[IC] |= num<<TWO;
					IC++;
					}						
				}
			else if (sourceBitsCode == THREE && destBitsCode == ZERO){//source is a register and destination is a number
				regNum = checkOpernad(wordArray[j + ONE], labelCurr);
				commandTable[IC] |= regNum<<FIVE;
				IC++;
				num = checkNum(wordArray[j + TWO], labelCurr);
				commandTable[IC] |= num<<TWO;
				IC++;						
				}
			else if ((sourceBitsCode == ONE || sourceBitsCode == TWO || sourceBitsCode == FOUR) && (destBitsCode == ONE || destBitsCode == TWO ||destBitsCode == FOUR)){//both are labels
				if (sourceBitsCode == FOUR){//operand is an external label
					commandTable[IC] |= EXTERNAL;
					IC++;
					}
				else{
					labelAddress = checkLabelAddress(wordArray[j + ONE], labelCurr);//else it's a label
					commandTable[IC] |= RELOCATABLE;
					commandTable[IC] |= labelAddress<<TWO;
					IC++;
					}

				if (sourceBitsCode == TWO){
					num = checkNum(wordArray[j + ONE], labelCurr);
					commandTable[IC] |= num<<TWO;
					IC++;
					}
				if (destBitsCode == FOUR){//operand is an external label
					commandTable[IC] |= EXTERNAL;
					IC++;
					}
				else{
					labelAddress = checkLabelAddress(wordArray[j + TWO], labelCurr);//else it's a label
					commandTable[IC] |= RELOCATABLE;
					commandTable[IC] |= labelAddress<<TWO;
					IC++;
					}

				if (destBitsCode == TWO){
					num = checkNum(wordArray[j + TWO], labelCurr);
					commandTable[IC] |= num<<TWO;
					IC++;
					}						
				}
			else if ((sourceBitsCode == ONE || sourceBitsCode == TWO ||sourceBitsCode == FOUR) && destBitsCode == THREE){//source is a label and destination is a register
				if (sourceBitsCode == FOUR){//operand is an external label
					commandTable[IC] |= EXTERNAL;
					IC++;
					}
				else{
					labelAddress = checkLabelAddress(wordArray[j + ONE], labelCurr);//else it's a label
					commandTable[IC] |= RELOCATABLE;
					commandTable[IC] |= labelAddress<<TWO;
					IC++;
					}

				if (sourceBitsCode == TWO){
					num = checkNum(wordArray[j + ONE], labelCurr);
					commandTable[IC] |= num<<TWO;
					IC++;
					}
				regNum = CheckRegNum(wordArray[j + ONE]);
				commandTable[IC] |= regNum<<FIVE;
				IC++;						
				}
			else if ((sourceBitsCode == ONE || sourceBitsCode == TWO ||sourceBitsCode == FOUR) && destBitsCode == ZERO){//source is a label and destination is a register
				if (sourceBitsCode == FOUR){//operand is an external label
					commandTable[IC] |= EXTERNAL;
					IC++;
					}
				else{
					labelAddress = checkLabelAddress(wordArray[j + ONE], labelCurr);//else it's a label
					commandTable[IC] |= RELOCATABLE;
					commandTable[IC] |= labelAddress<<TWO;
					IC++;
					}

				if (sourceBitsCode == TWO){
					num = checkNum(wordArray[j + ONE], labelCurr);
					commandTable[IC] |= num<<TWO;
					IC++;
					}
				num = checkNum(wordArray[j + TWO], labelCurr);
				commandTable[IC] |= num<<TWO;
				IC++;						
				}
			}
		else if (code == ONE){ //command receives 1 operands
			opcode = checkOpcode(wordArray[j]);	
			commandTable[IC] |= opcode<<SIX;
			destBitsCode = checkOpernad(wordArray[j + ONE], labelCurr);
			if (destBitsCode == FOUR)
				commandTable[IC] |= ONE<<TWO;
			else
				commandTable[IC] |= destBitsCode<<TWO;
			IC++;
 			if (destBitsCode == THREE){//operand is a register
				regNum = CheckRegNum(wordArray[j + ONE]);
				commandTable[IC] |= regNum<<TWO;
				IC++;		
			}
			else if (destBitsCode == ZERO){//operand is a number
				num = checkNum(wordArray[j + ONE], labelCurr);
				commandTable[IC] |= num<<TWO;
				IC++;
				}
			else if (destBitsCode == ONE || destBitsCode == TWO ||destBitsCode == FOUR){
				if (destBitsCode == FOUR){//operand is an external label
					commandTable[IC] |= EXTERNAL;
					IC++;
					}
				else{
					labelAddress = checkLabelAddress(wordArray[j + ONE], labelCurr);//else it's a label
					commandTable[IC] |= RELOCATABLE;
					commandTable[IC] |= labelAddress<<TWO;
					IC++;
					}

				if (destBitsCode == TWO){
					num = checkNum(wordArray[j + ONE], labelCurr);
					commandTable[IC] |= num<<TWO;
					IC++;
					}
				}
		}
		else if (code == ZERO){ //command receives 0 operands
			opcode = checkOpcode(wordArray[j]);	
			commandTable[IC] |= opcode<<SIX; //opcode								
			IC++;
			}
		}
	}
}

if (errorFound == false){	//files section
	printf("finished coding\n");
	labelCurr = *labelHead;
//An assisting function for printing the binary representation.Uncomment to see the binary output on the screen
	/*for (int i = ZERO; i < *ICallocation; i++)
		printBinary(commandTable[i], i);
	printf("data\n");
	for (int i = ZERO; i < *DCallocation; i++)
		printBinary(dataTable[i], i + *ICallocation);*/
	if (entFlag == true){
		strcpy(destfilename, filename);
		dot = strrchr(destfilename, '.');
		*dot = '\0'; 
		strcat(destfilename, ENTRY_FILE_EXTENSION);
	if ((fpent = fopen(destfilename, "w")) == NULL) {
		printf("Error. Failed to open file entry file\n");
       		exit(2);
		}
	createEntFile(fpent, labelCurr);
	fclose(fpent);
	printf("entry file closed succesfully\n");
	}
	if (extFlag == true){
		strcpy(destfilename, filename);
		dot = strrchr(destfilename, '.');
		*dot = '\0'; 
		strcat(destfilename, EXTERN_FILE_EXTENSION);
		if ((fpext = fopen(destfilename, "w")) == NULL) {
			printf("Error. Failed to open file extern file\n");
        		exit(2);
			}
		createExtFile(fpext, labelCurr);
	fclose(fpext);
	printf("extern file closed succesfully\n");
	}

	strcpy(destfilename, filename);
	dot = strrchr(destfilename, '.');
	*dot = '\0'; 
	strcat(destfilename, OBJECT_FILE_EXTENSION);
	if ((fpobj = fopen(destfilename, "w")) == NULL) {
		fprintf(stderr,"Error. Failed to open file entry file\n");
        	exit(2);
	}
	createObjectFile(fpobj,  *DCallocation, *ICallocation, dataTable, commandTable);
	fclose(fpobj);
	printf("Object file closed succesfully\n");
}
else
	printf("Errors were found in the second run. Files will not be produced\n");
	
}

/*while (labelCurr){
	printf("label name %s, value %d type %d\n", labelCurr->Name, labelCurr->value, labelCurr->symbol);labelCurr=labelCurr->next;}*/
				
