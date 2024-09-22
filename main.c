#include "project_defines.h"


int main (int argc, char *argv[]) {
    FILE *fp, *fpam;//File pointers for the program uses
    int i = ZERO, *IC, *DC;
    char *filename = NULL;
    bool firstRunComplete = false, macroComplete = false;
    label *labelMain = NULL;

IC = ZERO; 
DC = ZERO;
    //verify command line arguments 
    if (argc == 1){
        fprintf(stderr, "ERROR: You must enter at least 1 assembly (%s) input file name as a command line argument\n", ASSEMBLY_FILE_EXTENSION);
        exit(2);
   	 }
	//check file opens correctly
    for (i = ONE; i < argc; i++){ 
	filename = (char*)realloc(filename,strlen(*++argv)+THREE);
	if (filename == NULL){
		printf("Failed to allocate memory for filename!\n");
            	exit(2);
		}
	strcpy(filename,*argv);
        strcat(filename, ASSEMBLY_FILE_EXTENSION);
 	if ((fp = fopen(filename, "r")) == NULL){
		printf("Error. Failed to open file. File no. %d\n", i);
		exit(2); 
		}
	printf("File %d opened successfully\n", i);
	//assign new files extensions for the macro deployment
	strcpy(filename,*argv);
        strcat(filename, MACRO_FILE_EXTENSION);
        //open macro deployment files
        if ((fpam = fopen(filename, "w+")) == NULL) {
		printf("Error. Failed to open file macro file\n");
            exit(2);
        	}
        printf("Deploying macros %s\n", filename);
	macroComplete = deployMacro(fp, fpam);
	if (macroComplete == true){
        	printf("Macros deployed succesfully\n");
        	printf("initiating first run\n");
		firstRunComplete = firstrun(fpam, &labelMain, &IC, &DC);
        	if (firstRunComplete == true) {
          		 printf("First pass run completed successfully on file %s\n", filename);
           		 printf("Running second pass on file %s\n", filename);
			 secondrun(fpam, filename, &labelMain, &IC, &DC);
			}
        	else 
            		fprintf(stderr, "ERROR: Errors found in first pass. Second pass will not be run.\n");

            printf("INFO: Finished running assembler on file %s\n", filename);
            printf("-----------------------------------------------------------------\n");
        }

free_label(&labelMain);
printf("memory released successfully\n");
	fclose(fp);
	printf("Original file closed succesfully\n");
	fclose(fpam);
	printf("Macro file saved successfully\n");
    }

    return ZERO;
}
