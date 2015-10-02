#include "depends.h"
#include "userdef.h"
#include "directories.h"

int main( )
{
	int t, len, i , l , j ;
	int status;
	char ch;

	user_details = getpwuid(getuid());
    getcwd( pCWD, sizeof(pCWD));
    home = (char *)malloc(sizeof(char)*LIMIT);

    while( pCWD[i] != '\0')                         // Initialize the home directory to the PWD
    {
        home[i] = pCWD[i];
        i++;
    }

	preProcess();

	while( 1)
	{
		initialize();

		directIN[0] = directOUT[0] = '\0';
		
		in = out = 0;

		int cmd_count;

		for(cmd_count = 0; cmd_count < cmd_num ; cmd_count++ )
		{
			i = j = 0;

			strcpy(str,commands[cmd_count]);
			strcpy(backup , str);
	        
	        if( checkforpipe( str ) )
				continue;

			if( strstr( str , "quit" ) != NULL )
			{
				write( 1 , "Exiting Shell Now\n" , 18 );
				exit(0);
			}

			len = strlen( str );
			parse( str , tokenized  );
			
	        if( strstr( tokenized [0] , "cd") != NULL )
				kardoCD( tokenized  );

			else if( strstr( tokenized [0] , "echo") != NULL )
				echo( tokenized  );

			else if( strstr( tokenized [0] , "pwd" ) != NULL )
				dispPWD();
			
			else if( strstr( tokenized [0] , "pinfo") != NULL )
				procInfo( tokenized );
			
			else if( strstr( tokenized [0] , "jobs") != NULL )
				jobs( );
			
			else if( strstr( tokenized [0] , "overkill") != NULL )
				overkill( );
			
			else if( strstr( tokenized [0] , "kjob") != NULL )
					killJob( atoi( tokenized [1]) , atoi( tokenized [2]));

			else if( strstr( tokenized [0] , "fg") != NULL )
				fg( atoi( tokenized [1]));
			
			else
			{
				pid = fork( );

				if( pid == 0)
				{
					if( in == 1 )
					{
						FILE *opened = fopen( directIN , "r");
						dup2( fileno( opened ) , 0);
					}
					if( out == 1 )
					{
						FILE *opened = fopen( directOUT , "w+");
						dup2( fileno( opened ) , 1);
					}

					setpgid( 0 , 0);
					
					if( execvp( tokenized [0] , tokenized ) < 0 )
						printf( "No such command exist\n");

					exit( 0);
				}
				else if( pid>0)
				{
					if( background == 0)
					{
						int newStatus;

						waitpid( pid , &newStatus , WUNTRACED);

						if( WIFSTOPPED( newStatus))
						{
							int cnt = 0;
							
							while( *( tokenized + cnt ) != '\0' )
							{
								strcpy( nodeList[stackPointer].procName[cnt] , *( tokenized +cnt ));
								cnt++;
							}

							modifyList( cnt );
						}
					}
					
					else
					{
						j = 0;

						while( *( tokenized + j ) != '\0')
						{
							strcpy( nodeList[stackPointer].procName[j] , *( tokenized +j));
							j++;
						}
						modifyList( j );
					}
				}

				else
				{
					perror("An Error Occured\n");
				}
			}
		}
	}

	tcsetattr( 0 , TCSANOW , &old);
	return 0;
}

void preProcess()
{
	tcgetattr( STDIN_FILENO ,  &old);
	tcgetattr( STDIN_FILENO ,  &new);
	tcsetattr( 0 , TCSANOW , &new);
	getcwd( execPath , LIMIT );
	presentdir( execPath , PWD);

}

void modifyList( int j )
{
	strcpy( nodeList[stackPointer].procName[j] , "NULL");
	nodeList[stackPointer].procID = pid;
	nodeList[stackPointer].procStatus = 1;
	stackPointer++;
}

void parse( char *userInput ,  char *execArgs[])
{
	int i , j , k , lmt , intro ;
	char *userCMD = userInput;

	while( *userCMD == ' ' || *userCMD == '\n' || *userCMD == '\t' || *userCMD == '\0')
		userCMD++;

	int counter = 0 , counter_1 = 0;
	int flag = 0 ;
	
	while( *userCMD != '\0')
	{

		if( *userCMD == '<')
		{
			*execArgs = '\0';
			counter_1 = 0;
			in = 1;

			*userCMD = '\0';userCMD+= 1;
			
			while( *userCMD == ' ' || *userCMD == '\n')
				userCMD++;

			while( *userCMD != ' ' && *userCMD != '>' && *userCMD != '\t' && *userCMD != '\n')
			{
				directIN[counter_1] = *userCMD;
				userCMD++;
				counter_1++;

				if( *userCMD == ' ' || *userCMD == '\n')
				{
					flag = 1;
					break;
				}
			}

			directIN[counter_1] = '\0';
			
			while( *userCMD == ' ')
				userCMD++;
			
			printf( "%d\n" , flag);
			
			if( flag )
				break;
		}

		else if( *userCMD == '>')
		{
			out = 1 ;
			counter_1 = 1;

			*execArgs = '\0';
			*userCMD = '\0';
			userCMD++;
			
			while( *userCMD == ' ' || *userCMD == '\0')
				userCMD++;
			
			while( *userCMD != ' ' && *userCMD != '\0' && *userCMD != '\n' && *userCMD != '<' )
			{
				directOUT[counter_1] = *userCMD;
				userCMD++;
				counter_1++;

				if( *userCMD == '\0' || *userCMD == '\n')
				{
					flag = 1;
					break;
				}
			}

			if( *userCMD == ' ')
				userCMD++;
			
			directOUT[counter_1] = '\0';
			
			if( flag)
				break;
		}
		
		else if( *userCMD != ' ' && *userCMD != '&')
		{
			*execArgs  =  userCMD;
		}
		
		while( *userCMD != ' ' && *userCMD != '\t' && *userCMD != '\0' && *userCMD != '\n')
		{
			if( *userCMD == '&')
			{
				background = 1;
				*execArgs = '\0';
				*userCMD = '\0';
				break;
			}
			userCMD++;
		}

		if( flag )
			break;

		while( ( *userCMD  ==  ' ' || *userCMD  ==  '\t' || *userCMD  ==  '\n'))
		{
			*userCMD  =  '\0';
			userCMD++;
			if( *userCMD == '\0')
				break;
		}
		execArgs++;
	}
	*execArgs = '\0';
}

void initialize()
{
	int i;

	background = 0;

	str = ( char * )malloc( sizeof( char ) * LIMIT );
	
	for( i = 0; i < LIMIT ; i++)
		tokenized [i] = ( char* )malloc( sizeof( char )* LIMIT );
	
	if ( signal( SIGCHLD , child_handler ) == SIG_ERR )
		perror("SIGNAL ERROR");

	if ( signal( SIGINT ,  SIG_IGN ) == SIG_ERR )
		perror("SIGNAL ERROR");
	
	if ( signal( SIGQUIT , sigHandler ) == SIG_ERR )
		perror("SIGNAL ERROR");

	if ( signal( SIGINT , sigHandler ) == SIG_ERR )
		perror("SIGNAL ERROR");

	if ( signal( SIGTSTP , sigHandler ) == SIG_ERR )
		perror("SIGNAL ERROR");

	user_details = getpwuid( getuid() );
	
	printPrompt2( user_details );
	
	getline( &str , &BUFSIZE , stdin);

	strcpy( input , str);
	cmd_num = parseInput();
}

void callPipe( char *userInput )
{
	int streamIN , streamOUT ;
	int i, l, counter, j ;
	int stat;
	
	i = 0;
	l = 0;
	counter = 0;
	j = 0;
	stat = 0;

	char *inp = userInput , *newText;
	char *exexCMD[ LIMIT ];

	for ( i = 0 ; i < LIMIT ; i++ )
		exexCMD[i] = ( char* )malloc(sizeof( char ) * LIMIT);

	newText = strtok( inp ,  "|");

	while( newText != NULL)
	{
		newText[strlen( newText)] = '\0';
		strcpy( exexCMD[counter] ,  newText );
		counter++;
		newText = strtok( NULL ,  "|");
	}

	streamIN = dup( STDIN_FILENO);
	streamOUT = dup( STDOUT_FILENO);
	
	for( i = 0; i < counter ; i++ )
	{
		in = out = 0;

		char *brokenCMD[ LIMIT ];
		parse( exexCMD[i] , brokenCMD);
		
		j = 0;
		
		if( i < counter-1 )
			pipe( finalPipe );
		
		pid = fork();
	
		if( i > 0 && i <= counter-1)
		{
			dup2( initialPipe[0] , 0 );
			close( initialPipe[0] );
			close( initialPipe[1] );
		}
		
		if( pid == 0)
		{

			if( i >= 0 && i < counter-1 )
			{
				dup2( finalPipe[1] , 1);
				close( finalPipe[0] );
				close( finalPipe[1] );
			}

			if( in == 1)
			{
				FILE *opened = fopen( directIN , "r");
				dup2( fileno( opened ) , 0);
				in = 0;
			}

			if( out == 1)
			{
				FILE *opened = fopen( directOUT , "w+");
				dup2( fileno( opened ) , 1);
				out = 0;
			}

			stat = execvp( *brokenCMD , brokenCMD);

			if( stat < 0 )
				write( 2 , "Piping Error\n" , 13 );
		}
		
		else if( pid>0)
		{
			int status;
			waitpid( pid ,  &status ,  0);
			if( i<counter-1)
			{
				initialPipe[0] = finalPipe[0];
				initialPipe[1] = finalPipe[1];
			}
		}
		
		else
			write( 2 , "Unexpected Error Occured\n" , 25 );

	}

	dup2( streamIN ,  0);
	dup2( streamOUT ,  1);
	
	close( initialPipe[0]);
	close( finalPipe[0]);
	close( initialPipe[1]);
	close( finalPipe[1]);
	
	close( streamIN );
	close( streamOUT);
}

void child_handler( int sigNumber )
{	
	int status;
	int i , j , k , counter , flag;
	int condition = waitpid( -1 , &status , WNOHANG);
	
	if( condition > 0)
	{
		for( j = 0 ; j < stackPointer ; j++)
		{
			if( nodeList[j].procID == condition)
			{
				nodeList[j].procStatus = 0;
				printf( "\n %s with PID %d exited normally\n" , *nodeList[j].procName , nodeList[j].procID );
				printPrompt2( user_details );
				fflush( stdout);
				break;
			}
		}
	}
	signal( SIGCHLD , SIG_IGN);
}

void sigHandler( int sigNumber )
{
	if( sigNumber == 20 )
	{
		// SIGTSTP == 20
		kill( pid , 20 );
	}

	else if( sigNumber == 2 || sigNumber == 3 )
	{
		fflush( stdout );
		write( 1 , "\n" , 1 );
		printPrompt2( user_details );
		fflush( stdout );
		signal( SIGQUIT , sigHandler );			// Ctrl + c
		signal( SIGTSTP , sigHandler );			// Ctrl + z
	}
	return ;
}

int checkforpipe( char *inputSTR )
{
	char *input = inputSTR ;
	int flag = 0;
	char c = '|';

	while( *input != '\n' && *input != '\0')
	{
		if( *input == c )
		{
			callPipe( inputSTR  );
			flag = 1;
			break;
		}

		input++;
	}
	return flag;
}

int parseInput()
{
    int i;
    int counter = 0;
    for(i=0;input[i]!='\0';i++)
        if(input[i] == ';')
            counter++;

    i = 0;
    cmd = strtok(input, ";");
    while( cmd != NULL ) 
    {
        commands[i] = cmd;
        cmd = strtok(NULL, ";");
        i++;
    }
    if ( i==1 )
        commands[0] = input;
    return counter+1;
}