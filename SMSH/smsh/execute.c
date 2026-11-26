/* execute.c - code used by small shell to execute commands */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<signal.h>
#include	<sys/wait.h>
#include	"varlib.h"
#include	"smsh.h"
#include    <string.h>



int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
	int	pid ;
	int	child_info = -1;

	if ( argv[0] == NULL )		/* nothing succeeds	*/
		return 0;

	if ( (pid = fork())  == -1 )
		perror("fork");
	else if ( pid == 0 ){
		parse_variable_extension(argv);
		quoted_characters(argv);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execvp(argv[0], argv);
		perror("cannot execute command");
		exit(1);
	}
	else {
		if ( wait(&child_info) == -1 )
			perror("wait");
	}
	return child_info;
}

void parse_variable_extension(char* argv[]){
	for(int i = 1; argv[i] != NULL; ++i){
		if (argv[i][0] == '$'){

			//checks for .extensions
			if (strchr(argv[i], '.') != NULL){
				int j = 1;
				char new_arg[32];
				while(argv[i][j] != '.'){
					new_arg[j-1] = argv[i][j];
					++j;
				}
				new_arg[j-1] = '\0';
				char* val = VLlookup(new_arg);
				if (val != NULL) {
					char* extension = &argv[i][j];
					char* combined = malloc(strlen(val) + strlen(extension) + 1);

					int k = 0;
					while(val[k] != '\0'){
						combined[k] = val[k];
						++k;
					}

					int n = 0;
					while (extension[n] != '\0'){
						combined[k+n] = extension[n];
						++n;
					}
					combined[k+n] = '\0';
					argv[i] = combined;
				}
			} else{
				argv[i] = VLlookup(&argv[i][1]);
			}
		}
	}
}

void quoted_characters(char* argv[]){
	if (strchr(argv[1], '\\') != NULL){
		int i = 1;
		int read_index = 0;
		for (int j = 0;  argv[i][j] != '\0'; ++j, ++read_index){
			if(argv[i][j] == '\\' && read_index == j){
				argv[i][j] = argv[i][++read_index];
			} else if(argv[i][read_index] == '\\'){
				argv[i][j] = argv[i][++read_index];
			} else
				argv[i][j] = argv[i][read_index];
		}
	}
}