#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include"commands.h"

#define tok_bufsize 64
#define tok_delim " \t\n"

char *read_line(void)
{
    char *line=NULL;
    ssize_t bufsize=0;
    getline(&line, &bufsize, stdin);
	strcat(istoric, line);
	strcat(istoric, "\0");    
	return line;
}

char **split_line(char *line)
{
    int bufsize=tok_bufsize;           //constanta pentru alocare/realocare dimensiune tokens
    int pos=0;
    char **tokens=malloc(bufsize*sizeof(char*));
    char *token;

    if(!tokens)
    {
        printf("Allocation error\n");
        exit(EXIT_FAILURE);
    }
    token=strtok(line, tok_delim);
    while(token!=NULL)
    {
        tokens[pos]=token;
        pos++;
        if(pos>=bufsize)
        {
            bufsize+=tok_bufsize;
            tokens=realloc(tokens, bufsize*sizeof(char*));
            if (!tokens)
            {
                printf("allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token=strtok(NULL, tok_delim);
    }
    tokens[pos]=NULL;  //ptr ca folosim execve
    return tokens;
}

int cmdexecute(char **args);

void loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        line=read_line();
        args=split_line(line);
        status=cmdexecute(args);

        free(line);
        free(args);
    }
    while(status);
}

int launch(char **args)
{
    pid_t pid, wpid;
    int status;
    pid=fork();
    if (pid==0)
    {	
	//child process
        if (execvp(args[0], args)==-1)
        {
            perror("not executed");
        }exit(EXIT_FAILURE);
	
    }
    else if(pid<0)
        perror("error forking");
    else
	{
		printf("Not from our shell command\n");
        	do
        	{
        	    wpid=waitpid(pid, &status, WUNTRACED);
        	}
        	while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	 return 1;
}

int ourlaunch( int (*myfn)(char**), char **args)
{
	pid_t pid, wpid;
    int status; 
    pid=fork();
    if (pid==0)
    {  
        //child process
	int retCode = (*myfn)(args);
        if (retCode != 1)
        {
            perror("not executed");
            exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
	
    }
    else if(pid<0)
        perror("error forking");
    else
        {
                do
                {
                    wpid=waitpid(pid, &status, WUNTRACED);
                }
                while(!WIFEXITED(status) && !WIFSIGNALED(status));
		
        }
         return 1;
}

int cmdexecute(char **args)
{
    int i;
    if(args[0]==NULL)
        return 1;
    for (i=0; i<num_builtins(); i++)
    {
        if(strcmp(args[0], builtin_names[i])==0)
        {
	    if(strcmp(args[0], "ourexit") == 0)
	        return(*builtin_func[i])(args);
	    return ourlaunch(*builtin_func[i], args);    
        }
    }
   return launch(args);
}

int main(int argc, char **argv)
{
	istoric=(char *)malloc(100*sizeof(char));
	istoric[0]='\0';   
 	loop();
    return EXIT_SUCCESS;
}
