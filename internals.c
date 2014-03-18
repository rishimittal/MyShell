

/* Internals.c 
   Consists all the internal function call definitions.
*/


#include "pHeader.h"

/*Internal Command : cd <path> */
void executeCd(char** argvCom ){
	char res[200]; 
	char* user = getenv("USER");
	strcpy(res ,"/home/");
	strcat(res ,user);

	 if(argvCom[1]  == NULL){
		if(chdir(res) != 0){
			perror("CHDIR ERROR");
		}
	 }else {
		 if(chdir(argvCom[1]) != 0){
                        perror("CHDIR ERROR");
                }
	
	 }	
	
}
/*Internal Command : pwd*/
void executePwd(char** argvCom,int inFile ,int outFile) {
	char cwd[500];
	char res[510];
        if(getcwd(cwd , sizeof(cwd)) == NULL){ /*Get curent working directory call*/
             perror("chd");
	}
	strcpy(res,cwd);
	strcat(res ,"\n");
	write(outFile , res ,(strlen(res) + 1));
}

/* Internal Command Export Variables*/
void executeExport(char** argvCom){
	char key[500];
	char value[1000];
	int j , k;

	j = 0;
	while(argvCom[1][j] != '='){
		key[j] = argvCom[1][j];
	j++;
	}
	key[j] = '\0';
	j++;
	k = 0;
	while(argvCom[1][j] != '\0'){
		value[k] = argvCom[1][j];
	k++;
	j++;
	}
	value[k] = '\0';
	//printf("%s->%s",key,value);
	
	if(setenv(key ,value,1) != 0){
		perror("SET ENV ERROR");
	}
	//char* vc = getenv("RI");
	//printf("%s\n" ,vc);
}

/* Internal Command : export <name=path> */

void executeEcho(char** argvCom , int inFile , int outFile) {
	  char bef[1000];
	  char aft[1000];
	  char *vc;
	  char outp[2000];
	  int j ,k;

	  j = 0;
          while(argvCom[1][j] != '$'){
                  bef[j] = argvCom[1][j];
          j++;
          }
          bef[j] = '\0';
	  j++;
          
	  k = 0;
          while(argvCom[1][j] != '\0'){
                  aft[k] = argvCom[1][j];
          k++;
          j++;
          }
          aft[k] = '\0';
	  //printf("%s",aft);
	 // printf("%s->%s",bef,aft);
	 if((vc= getenv(aft)) == NULL){
		printf("\n");
	 }else{
	 	strcpy(outp,bef);
		strcat(outp,vc);
         	strcat(outp ,"\n");
         	write(outFile , outp ,(strlen(outp) + 1));
	 
	 }
}

/* Load the .AvgRc(configuration file) file*/
/* Executes all the commands line by line */
bool loadAgvRc(char** argvPipe, char** argvCom ,int* cMemArgvPipe,int* cMemArgvCom,char* tokenPipe,char* tokenSpace){
	bool rload = false;
	int ofd;
	char** hist = (char**)malloc(10 * sizeof(char*));
	hist[0] = (char*)malloc(10*sizeof(char));
	hist[1] = NULL;
	if( (ofd = open("./.AvgRc" ,O_RDONLY)) == -1){
		perror("FILE NOT FOUND OR PERMISSION DENIED");
		rload = false;
	}else {
		rload = true;
		char line2;
		char line[1024];
		int i = 0 ,j = 0;
		
		
		while( (i = read(ofd , &line2 , 1) )  > 0 ){
			
			if(line2 == '\n' || line2 == ':'){
				line[j] = '\0';
				processData(line ,argvPipe,hist,argvCom,cMemArgvPipe, cMemArgvCom,tokenPipe,tokenSpace); 
				j = 0;
			}else{
				line[j++] = line2;
			}
		}
		close(ofd);
	}
return rload;
}

/*Usage of History Operator*/
void executeHistory(char** hist ,int inFile , int outFile){
	int x = 0;
	while(hist[x] != NULL){
		char result[1024];
		sprintf(result , " %d  %s\n",x + 1 ,hist[x]);
		write(outFile ,result , (strlen(result) + 1));
	x++;
	}
}
/* Waiting when Fg is called */
void executeFg(char** argvCom){
	int stat;
	while( (stat = waitpid((pid_t)atoi(argvCom[1]) ,&stat , 0)) == 0){}
}

/* usage of Bang : cases handled : !n ,!-n ,!!*/
void executeBang(char* lineCopy,char** hist ,char** argvPipe,char** argvCom,int* cMemArgvPipe,int* cMemArgvCom,char* tokenPipe,char* tokenSpace){
	//printf("!control");
	char com[1024];
	int j = 1, i = 0;
	int hcount = 0;
	int histPreChek  = 0;
	//printf("%s",lineCopy);
	while(lineCopy[j] != '\0'){
		com[i] = lineCopy[j];
		//printf("%c\n",com[i]);
	i++;
	j++; 
	}
	com[i] = '\0';
	//printf("%s" ,com);
	i = 0;
	while(hist[i] != NULL){
		hcount++;
		if((strcmp(hist[i] ,com)) == 0){
			histPreChek = 1;
		}
	i++;
	}
	
	long int ret;
	char *ptr;
	ret  = strtol(com ,&ptr ,10);
	
	if(ret > 0 && *ptr == '\0'){
		long int index = ret % 1024;
		//printf("%ld",index);
		//printf("%s\n" ,hist[index - 1] );	
		processData(hist[index -1] ,argvPipe, hist, argvCom,cMemArgvPipe, cMemArgvCom,tokenPipe,tokenSpace);
	}else if(ret < 0 && *ptr == '\0'){
		long int nindex = hcount + (ret % 1024) ;
                //printf("%ld" , nindex);
                //printf("%s\n" , hist[nindex - 1]);
		processData(hist[nindex -1] ,argvPipe, hist, argvCom,cMemArgvPipe, cMemArgvCom,tokenPipe,tokenSpace);
	}else if(ret == 0 && *ptr != '!'){
		if(histPreChek == 1) {
			processData(com,argvPipe, hist, argvCom,cMemArgvPipe, cMemArgvCom,tokenPipe,tokenSpace);
		}else if(histPreChek == 0){
			printf("Not found in recent history.\n");
		}
	}

	//printf("%ld\n",ret);
	//printf("%c",*ptr);

}





