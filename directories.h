void printPrompt2(struct passwd *user_details) 
{
    username = getlogin();
    gethostname( hostname, 1024);
    getcwd( pCWD, sizeof(pCWD));

    //home = user_details->pw_dir;
    //strcpy( home , pCWD );
    //trimCWD();

    editCWD();
    if (pCWD != NULL)
        fprintf( stdout, "%s@%s:%s>", username, hostname, pCWD);
    else
        perror("getpCWD() error");

}

void trimCWD()
{
    char c = '/';
    char trimmed[LIMIT] ;
    int counter = 0;
    int i , j;
    for( i = 0 ; pCWD[i] != '\0'; i++ )
    {
        if ( pCWD[i] == '/' )
            counter = i;
    }
    for( i=counter+1 , j=0; pCWD[i]!='\0' ; i++ , j++ )
    {
        trimmed[j] = pCWD[i];
    }
    trimmed[j] = '\0';
    strcpy( pCWD, trimmed );
}

void editCWD()
{
    char *temp;
    temp = ( char *)malloc(sizeof(char) * LIMIT);
    int i, j, counter;
    
    j = i = 0;
    
    if (strlen(pCWD) >= strlen(home))
    {
        while( home[i] == pCWD[i] )
            i++;
        temp[j] = '~';
        i--; 
        if( !pCWD[i] == '\0')
            i++;
        j++;
        while( pCWD[i] != '\0' )
        {
            temp[j++] = pCWD[i++];
        }
        temp[j++] = '\0';
        strcpy( pCWD, temp );
        pCWD[j] = '\0';
    }
}

//Finding out the original directory where the shell was run

void presentdir( char fullPath[ LIMIT ] , char modified[ LIMIT ] )
{
	int i = 0 ; 
	int j , counter;
	int x = 0;

	j = strlen( fullPath );

	for( i = 0; i < j ; i++)
	{
		if( fullPath[i] == '/')
			counter = i;
	}

	counter += 1;

	for( i = counter; i < j; i++)
		modified[x++] = fullPath[i];
	
	modified[x] = '\0';
}
