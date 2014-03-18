

/* pHeader.h
   contains all the definitions and the header files associated.
*/

/*HEADER FILES*/

#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<wait.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>

void processData(char* line , char** argvPipe,char** hist,char** argvCom ,int* cMemArgvPipe,int* cMemArgvCom,char* tokenPipe,char* tokenSpace);
//Internal Command : cd
void executeCd(char** argvCom);
//Internal Command : pwd
void executePwd(char** argvCom,int inFile ,int outFile);
//Internal Command : export
void executeExport(char** argvCom);
//Echo Env Variables;
void executeEcho(char** argvCom , int inFile , int outFile);
//Load the configuration file
bool loadAgvRc(char** argvPipe, char** argvCom ,int* cMemArgvPipe, int* cMemArgvCom,char* tokenPipe,char* tokenSpace);
//Prints the History data
void executeHistory(char** hist ,int inFile, int outFile);
//Execution of bang
void executeBang( char* line ,char** hist ,char** argvPipe,char** argvCom,int* cMemArgvPipe,int* cMemArgvCom,char* tokenPipe,char* tokenSpace);
//Execution of fg
void executeFg(char** argvCom);
//Execution of openhistoryfile
int openHistoryFile(int ffd);
//Execution of addHistory
bool addHistory(int fd , char* line); 
//Load the history file
bool loadHistoryFile(char** hist);
//output redirection
int outOp(char** argv);
char** argvBefOp(char** argv);
//Input redirection
int inOp(char** argv);
char** argvBefInOp(char** argv);


