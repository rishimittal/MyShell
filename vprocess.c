/*VPROCESS.C(MAIN PROGRAM)*/

#include "pHeader.h"

void executeProcess(char **argv, int inFile ,int outFile,int iflag){
        pid_t pid;
	int status;
	//printf("control");
	//printf("%s",argv[0]);	
	/*Creates the Child Process*/
        pid = fork();
        if(pid < 0) { /*fork Error*/
                fprintf(stderr, "Fork Failed");
                exit(1);
        }
        else if(pid == 0) { /*Child Process*/
                /*Duplicate the File descriptors if they are 
		  not the standard descriptors.*/
		if(inFile != 0){
			dup2(inFile , 0);	
		}	
		if(outFile != 1){
			dup2(outFile ,1);
		}
	
		/*Child Execution ,with new process*/
		execvp(argv[0] ,argv);
		perror("ERROR IN EXECVP");
		exit(0);
	}
        else { /*Parent Process*/
		int i = 1;
		if(iflag == 0) {
                   waitpid(pid , &status ,0);
                }else{
		    printf("[%d] %d\n" ,i++,(int)pid);	
		}
		//printf("Child Complete");
        }
}

int main(){
	bool load = false;
	bool hadd = false,hload = false;
	char *line,*tokenPipe,*tokenSpace;
	//Used to store data breaked on the bass of the | symbol
	char **argvPipe = (char**) malloc(200 * sizeof(char*));
	int cMemArgvPipe[1];
	//Used to store the data broken on the basis of the " ".
        char **argvCom = (char**)malloc(200 * sizeof(char*));
	char **hist = (char**)malloc(1024 * sizeof(char*));
	int cMemArgvCom[1];
	line = (char *)malloc( 1024 * sizeof(char));
	int ffd = 0;
	//Loop till exit is entered
	load = loadAgvRc(argvPipe,argvCom,cMemArgvPipe,cMemArgvCom,tokenPipe,tokenSpace);
	ffd = openHistoryFile(ffd);
	hload = loadHistoryFile(hist);
	if(load == false || ffd == -1 || hload == false) {
		exit(1);
	}
	while(1) {
		char cwd[500];
        	if(getcwd(cwd , sizeof(cwd)) == NULL) /*Get curent working directory call*/
                	perror("chd");
        	printf("Avengers:%s$ " ,cwd);	
		gets(line);
		//printf("%d\n",ffd);
		if(strcmp(line , " ") != 0 && line[0] != '!'){
			hadd = addHistory(ffd,line); //adds the history in the file.
		}
		hload = loadHistoryFile(hist);
		//if(hadd == false){
		//	continue;
		//}		
		//if(hload == false){
		//	printf("history file not loaded");
		//	continue;
		//}
		if(strcmp(line , "&") == 0){
			printf("INVALID & USAGE\n");
			continue;
		}
		processData(line ,argvPipe,hist,argvCom,cMemArgvPipe, cMemArgvCom,tokenPipe,tokenSpace);
	} 

	/* Closes history file fd*/
	close(ffd);
	 /* frees the Entire Dynamically allocated
           memory, Calls at time of Shell Closing.*/
        int b ,c;
        free(line);
        for(b = 0 ; b < cMemArgvPipe[1] ; b++)
                free(argvPipe[b]);
        for(c = 0 ; c < cMemArgvCom[1] ; c++)
                free(argvCom[c]);
	//for()
        free(argvPipe);
        free(argvCom);

return 0;
}
	
void processData(char* line , char** argvPipe,char** hist,char** argvCom ,int* cMemArgvPipe,int* cMemArgvCom ,char* tokenPipe,char* tokenSpace){

		int totalPipes = 0;
		char* lineCopy = (char*)malloc(sizeof(char) * 1024);
		strcpy(lineCopy , line);
	
                int countPipes = 0;
		int iflag = 0;

		tokenPipe = strtok(line ,"|");
		int i = 0;
		while(tokenPipe != NULL ) {
			//printf("%s\n" , tokenPipe);
				argvPipe[i] = (char*) malloc(200 * sizeof(char));
				(cMemArgvPipe[0])++; //Used to free memory
				strcpy(argvPipe[i] ,tokenPipe);
				tokenPipe = strtok(NULL , "|");
				countPipes++; 
			//}
		i++;
		}
		argvPipe[i] = NULL;
		//printf("%d\n" , countPipes - 1);
		countPipes = countPipes - 1;	
		totalPipes = countPipes;
		
		int flagbreak = 0;
		int k = 0;
                while (argvPipe[k] != NULL){
                      //printf("%s\n" , argvPipe[k]);
		      if(strcmp(argvPipe[k]," ") == 0) { printf("Invalid Command\n");exit(0);}
                k++; 
                }
			
		int status;	
		int my_pipe[2] ,inFile = 0, outFile = 1;
		i = 0;
		//printf("p4");
		while(argvPipe[i] != NULL) {
			//printf("p0");
			int status;
			tokenSpace = strtok(argvPipe[i] ," ");
			int j = 0;
			while(tokenSpace != NULL){
			//	printf("%s\n" ,tokenSpace);
				argvCom[j] = (char*)malloc(200 * sizeof(char));
				(cMemArgvCom[0])++; //Used to free Memory
				if(strcmp(tokenSpace ,"&") != 0){
					strcpy(argvCom[j] ,tokenSpace);
				}else{
					iflag = 1;
				}
				tokenSpace = strtok(NULL ," ");
			j++;
			}
			//printf("p1");
			if(iflag){
				argvCom[--j] = NULL;
			}else{
				argvCom[j] = NULL;
			}
			if(strcmp(argvCom[0] ,"exit") == 0){
				 printf("Bye...\n");
                        	 _Exit(0);
                   	}
			
			//int l = 0;
                	//while(hist[l] != NULL) {
                       	//	printf("%s\n",hist[l]);
                	//	l++;
                	//}
			
			//Alter the value for Multiple Pipes
			if(countPipes > 0){
				if(pipe(my_pipe) < 0) {
					perror("pipe");
					exit(1);
				}
				outFile = my_pipe[1];
				countPipes--;	
			}else{
				outFile = 1;
			}
			
			/* Manages Output Stream while redirection*/	 
 			int outFileRed = outOp(argvCom);
 			argvCom = argvBefOp( argvCom);
			if(outFileRed != -1){
				outFile = outFileRed;
			}
			 /* Manages Input Stream while redirection*/
			int inFileRed = inOp(argvCom);
			argvCom = argvBefInOp(argvCom);			
			if(inFileRed != -1){
				inFile = inFileRed;
			}
			
			//printf("Infile ->%d\n",inFile);
			//printf("OutFile-->%d\n",outFile);
			//printf("p2");
			if(argvCom[0][0] == '!' ){
                              executeBang(lineCopy,hist,argvPipe,argvCom,cMemArgvPipe,cMemArgvCom,tokenPipe,tokenSpace);
			      break;
                        }
			//printf(" lineCopy -> %s" ,lineCopy);
			// Create the child process & execute the argvCom
			if(strcmp(argvCom[0] ,"cd") == 0) {
				if(totalPipes == 0){
					executeCd(argvCom);
				}
			}else if(strcmp(argvCom[0] ,"pwd") == 0){
				executePwd(argvCom,inFile ,outFile);	
			}else if(strcmp(argvCom[0] , "export") == 0){
				executeExport(argvCom); //Assuming that no pipe is used
				break;			//after or before export command.
			}else if(strcmp(argvCom[0] ,"echo") == 0){
				executeEcho(argvCom, inFile, outFile);
			}else if(strcmp(argvCom[0] ,"history") == 0){
				executeHistory(hist ,inFile,outFile);
			}else if(strcmp(argvCom[0] ,"fg") == 0){
                                 executeFg(argvCom);
                        }else {
				//printf("p2");
				executeProcess(argvCom, inFile , outFile,iflag);
				  
			}
			//update the inFile status, if called twice , inFile is set to pipe end.
			if(inFile != 0)
				close(inFile);
			if(outFile != 1)
				close(outFile);
			inFile = my_pipe[0];
		i++;
		}
		inFile = 0;
		outFile = 1;
	       
} 
