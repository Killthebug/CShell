#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <termios.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

#define INITIAL 0
#define MAXPROCESS 32700
#define LENGTH 200
#define LIMIT 1024
#define MAX 256


typedef struct node
{
	char procName[ LIMIT ][ LIMIT ];
	char procexecPath[ LIMIT ];
	int completed;
    int stopped;
	int procID;
	int procStatus;
	int memory;
}node;

typedef struct job
{
    struct job *next;
    char *cmd;
    pid_t pgid;
    node *head_process;
    int confirm;
    int stdin;
    int stdout;
    int stderr;
    struct termios tmodes;
}job;

node nodeList[1000];//Defining array of structures
int stackPointer = INITIAL;
int background = INITIAL;
int in = INITIAL;
int out = INITIAL;
int initialPipe[2] ;
int finalPipe[2] ;
pid_t pid;
struct termios new;
struct termios old;
char directIN[1000];
char directOUT[1000];
char *str;
char *cmd;
struct passwd *user_details ;
char *home;              // Stores the home directory
char * commands[LIMIT];  // Stores the array of commands separated by a ";" 
char *tokenized[ LIMIT ];	 // Tokenize everything divided by spaces
char pCWD[LIMIT];        // Saves the name of the current working directory
char *username;          // Saves the username of the user logged in
char hostname[LIMIT];    // Gets the name of the system host 
int cmd_num;
char copy[LIMIT];
char input[LIMIT];
char backup[LIMIT];
size_t BUFSIZE ;

char PWD[1000];
char execPath[1000];

void preProcess();

void printPrompt();

void printPrompt2();

void editCWD();

void kardoCD( char *str1[1000] );

void presentdir( char a[1000] , char b[1000] );

void parse( char *inputString ,  char *execArgs[]);

void callPipe( char *input);

void child_handler( int sigNumber);

void sigHandler( int sigNumber);

void procInfo( char *str1[1000] );

void jobs( );

void killJob( int job , int signal );

void overkill( );

void fg( int ard );

int checkforpipe( char *inputSTR);

int parseInput();

void echo( char *broken[]);

void dispPWD();

void initialize();

void trimCWD();

void modifyList( int j );



