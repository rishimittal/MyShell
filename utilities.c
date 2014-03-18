

/* Utlities.c
   Contains all the utility methods defined.	
*/

#include "pHeader.h"


/* Adds the history in a file */
int openHistoryFile(int ffd) {
	if( (ffd = open("./.history" ,O_CREAT | O_APPEND | O_RDWR ,0644)) == -1){
                 //perror("FILE NOT FOUND OR PERMISSION DENIED");
         }  
return ffd;
 }

bool addHistory(int fd , char* line){
	bool hadd  = false;
	int wfd = 0;
	char com[1024];
	strcpy(com , line);
	strcat(com ,"\n");
	//printf("%s",com);
	if( (wfd = write(fd , com ,(strlen(line) + 1))) == -1){
		perror("HISTORY FILE WRITE ERROR");
	}else {
		hadd = true;
	}
return hadd;
}

bool loadHistoryFile(char** hist){
	bool hload = false;
	int fg;
        if( (fg = open("./.history" ,O_RDONLY)) == -1){
                //perror("FILE NOT FOUND OR PERMISSION DENIED");
        }else { 
                hload = true;
                char line2;
                char line1[1024];
                int i = 0 ,j = 0;
		int histIndex = 0;
                while( (i = read(fg , &line2 , 1) )  > 0 ){

                        if(line2 == '\n'){
                                line1[j] = '\0';
                                hist[histIndex] = (char*)malloc(sizeof(char) * 1024);
				strcpy(hist[histIndex] , line1);
				j = 0;
				histIndex++;
                        }else{
                                line1[j++] = line2;
                        }
                }
		hist[histIndex] = NULL;
                close(fg);

        }

return hload;
}

/* For the >> operators in a file */
int outOp(char** argv) {
	int j = 0,oloc = 0;
	int opfd = -1;
	while(argv[j] != NULL) {
                if(strcmp(argv[j] , ">>") == 0){
                        oloc = j + 1;
			if( (opfd = open(argv[oloc] ,O_CREAT | O_APPEND | O_RDWR ,0664)) == -1){
                 		perror("FILE NOT FOUND OR PERMISSION DENIED");
                 		exit(0);
       			 }
		}
		if(strcmp(argv[j] , ">") == 0){
                          oloc = j + 1;
                          if( (opfd = open(argv[oloc] ,O_CREAT | O_RDWR | O_TRUNC ,0664)) == -1){
                                  perror("FILE NOT FOUND OR PERMISSION DENIED");
                                  exit(0);
                           }      
                  }   
        j++;
        }
	
return opfd;
 }

char** argvBefOp(char** argv){
	int j;	
	char** newArgv = (char**)malloc(sizeof(char**) * 1024);

	j = 0;
        while(argv[j] != NULL) {
                if(strcmp(argv[j], ">>") == 0  || strcmp(argv[j] , ">") == 0 ) {
                        //printf("> found");
                        break;
                }else {
					
		      newArgv[j] = (char*)malloc(sizeof(char) * 1024);
                      strcpy(newArgv[j] , argv[j]);
		}
        j++;
        }
        newArgv[j] = NULL;

return newArgv;
}

/* For the > operators in a file */
int inOp(char** argv) {
        int j = 0,oloc = 0;
        int opfd = -1;
        while(argv[j] != NULL) {
                if(strcmp(argv[j] , "<") == 0){
                        oloc = j + 1;
			//printf("< found");
                        if( (opfd = open(argv[oloc] ,O_RDONLY )) == -1){
                                perror("FILE NOT FOUND OR PERMISSION DENIED");
                                exit(0);
                         }
                }
                if(strcmp(argv[j] , "<<") == 0){
                          oloc = j + 1;
			  char lines[1024];
                         if( (opfd = open("tempFile.txt" ,O_CREAT | O_RDWR | O_TRUNC ,0664)) == -1){
                                  perror("FILE NOT FOUND OR PERMISSION DENIED");
                                  exit(0);
                           }
			   printf(">");
			   gets(lines);
			   while( strcmp(lines ,argv[oloc]) != 0) {
				char result[1024];
				printf(">");
                		sprintf(result , "%s\n",lines);
                		write(opfd ,result , (strlen(result) + 1));
				gets(lines);	
			 }
			 close(opfd);
			 if( (opfd = open("tempFile.txt" ,O_RDONLY)) == -1){
                                  perror("FILE NOT FOUND OR PERMISSION DENIED");
                                  exit(0);
                         }	
                  }
        j++;
        }
return opfd;
 }

char** argvBefInOp(char** argv){
        int j;
        char** newArgv = (char**)malloc(sizeof(char**) * 1024);

        j = 0;
        while(argv[j] != NULL) {
                if(strcmp(argv[j], "<") == 0 || strcmp(argv[j] , "<<") == 0 ) {
                        //printf("> found");
                        break;
                }else {
                                        
                      newArgv[j] = (char*)malloc(sizeof(char) * 1024);
                      strcpy(newArgv[j] , argv[j]);
                }
        j++;
        }
        newArgv[j] = NULL;

return newArgv;
}

 
