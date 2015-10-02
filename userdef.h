void procInfo( char *execArgs[ LIMIT ])
{
	int i ,j ,k, cc;
	int pid;
	size_t length = 0;
	ssize_t read;
	ssize_t write;

	char localSTR[ LIMIT ];
	
	char *location, *BUF_1, *BUF_2;

	location = (char *)malloc(sizeof(char) * LIMIT );
	BUF_2 = (char *)malloc(sizeof(char) * LIMIT );
	BUF_1 = (char *)malloc(sizeof(char) * LIMIT );

	while( execArgs[i] != '\0')
		i++;
	
	if( i == 2)
		strcpy( localSTR , execArgs[1]);
	
	else if( i == 1)
	{
		pid = getpid( );
		sprintf( localSTR , "%d" , pid );
	}

	else
		perror("Invalid Arguments\n");

	if( i == 2 || i == 1)
	{
		int counter = 0 , c;
		length = 0;
		
		char ch , *line;

		strcpy( location , "/proc/");
		strcat( location , localSTR);
		strcpy( BUF_1 , location);
		strcat( location , "/status");
		
		FILE *fp = fopen( location ,  "r");
		
		if( fp )
		{
			while ( ( read  =  getline( &line ,  &length ,  fp))  !=  -1) 
			{
				counter++;
				if( counter <= 2 || counter == 4 || counter == 12)
					printf( "%s" , line);
			}
		}
		
		strcat( BUF_1 , "exe");
	}
}

void jobs( )
{
	int i , counter = 1 , j;
	for( i = 0; i < stackPointer ; i++)
	{
		if( nodeList[i].procStatus == 1)
		{
			printf( "[%d] " , counter);
			j = 0;
			while( strcmp( nodeList[i].procName[j] , "NULL") != 0)
			{
				printf( "%s " , nodeList[i].procName[j]);
				j++;
			}
			printf( "[%d]\n" , nodeList[i].procID );
			counter++;
		}
	}
}

void echo( char *broken[] )
{
	broken++;

	while( *broken != '\0')
		printf("%s ", *broken++ );
	
	printf("\n");
	return;
}

void dispPWD()
{
	char cwd[ LIMIT ];
	getcwd( cwd ,  LIMIT );
	printf("%s\n", cwd );
	return;
}

void killJob( int job ,  int sig)
{
	int i = 0 , counter = 0;
	
	for( i = 0;i<stackPointer;i++)
	{
		if( nodeList[i].procStatus == 1)
			counter++;

		if( counter == job)
		{
			kill( nodeList[i].procID , sig);
			break;
		}
	}
	return;
}

void overkill( )
{
	int i, j, count;

	for( i = 0; i < stackPointer ; i++)
		if( nodeList[i].procStatus == 1 )
		{
			nodeList[i].procStatus = 0;
			kill( nodeList[i].procID, 3 );
		}
	return;
}

void fg( int ard)
{
	int i , counter = 0 , flag = 0;
	int status ;

	for( i = 0; i < stackPointer ; i++)
	{
		if( nodeList[i].procStatus == 1)
			counter++;

		if( counter == ard)
		{
			flag = 1;
			
			nodeList[i].procStatus = 0;

			if( waitpid( nodeList[i].procID, &status , 0) > 0 )
				printf( "%swith pid %d exited normally\n" , nodeList[i].procName[0] , nodeList[i].procID);
		}
	}

	if( flag == 0)
		write( 2 , "The process with this count doesn't exist\n" , 42);
}


void kardoCD( char *execArgs[ LIMIT ])
{
	int i = 0;
	
	while( execArgs[i] != '\0' )
		i++;
	
	if( i == 2)
	{
		if( strcmp( execArgs[1] , "~") == 0)
			chdir( execPath );
		else
			if ( chdir( execArgs[1] ) == -1 )
				write( 2 , "Error > Directory not found\n" , 28 );
	}

	else if( i == 1)
		if( chdir( execPath ) == -1 ) 
			write( 2 , "Error > Couldn't change directory\n",34);

	else if( i >= 3 )
		write( 2 , "Error > Wrong Number of arguments\n" , 34);

	else
		return;	// Dummy Statement
}
