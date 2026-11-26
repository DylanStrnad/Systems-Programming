/* builtin.c
 * contains the switch and the functions for builtin commands
 */

#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	"smsh.h"
#include	"varlib.h"
#include <unistd.h>
#include <stdlib.h>

int assign(char *);
int okname(char *);

int builtin_command(char **args, int *resultp)
/*
 * purpose: run a builtin command 
 * returns: 1 if args[0] is builtin, 0 if not
 * details: test args[0] against all known builtins.  Call functions
 */
{
	int rv = 0;

	if ( strcmp(args[0],"set") == 0 ){	     /* 'set' command? */
		VLlist();
		*resultp = 0;
		rv = 1;
	}
	else if ( strchr(args[0], '=') != NULL ){   /* assignment cmd */
		*resultp = assign(args[0]);
		if ( *resultp != -1 )		    /* x-y=123 not ok */
			rv = 1;
	}
	else if ( strcmp(args[0], "export") == 0 ){
		if ( args[1] != NULL && okname(args[1]) )
			*resultp = VLexport(args[1]);
		else
			*resultp = 1;
		rv = 1;
	} 
	else if ( strcmp(args[0], "cd") == 0 ){ // change directory
		if (chdir(args[1]) != 0) { //any number other than 0 indicates a valid directory
                perror("invalid directory");
        }else
			rv=1;
	} else if (strcmp(args[0], "exit") == 0) {
		if (args[1] != NULL){ //check for exit number
			exit(args[1][0]);
		} else{
			exit(0);
		}
	} else if( strcmp(args[0], "read") == 0 ){
		if ( args[1] != NULL){
			const int SIZE = 30;
			char variable_Data[SIZE];

			if(fgets(variable_Data, SIZE, stdin) != NULL){
				VLstore(args[1], variable_Data);
				rv = 1;
			}
		} else {
			printf("need argument for read\n");
		}
	} 

	return rv;
}

int assign(char *str)
/*
 * purpose: execute name=val AND ensure that name is legal
 * returns: -1 for illegal lval, or result of VLstore 
 * warning: modifies the string, but retores it to normal
 */
{
	char	*cp;
	int	rv ;

	cp = strchr(str,'=');
	*cp = '\0';
	rv = ( okname(str) ? VLstore(str,cp+1) : -1 );
	*cp = '=';
	return rv;
}
int okname(char *str)
/*
 * purpose: determines if a string is a legal variable name
 * returns: 0 for no, 1 for yes
 */
{
	char	*cp;

	for(cp = str; *cp; cp++ ){
		if ( (isdigit(*cp) && cp==str) || !(isalnum(*cp) || *cp=='_' ))
			return 0;
	}
	return ( cp != str );	/* no empty strings, either */
}
