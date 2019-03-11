#ifndef HEADER_FILE
#define HEADER_FILE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
#include<dirent.h>

int shellcd(char **args);
int shellhelp(char **args);
int shellexit(char **args);
int shellls(char **args);
int shellhistory(char **args);
int shellpwd(char **args);
char *istoric;

char *builtin_names[]={"ourcd", "ourhelp", "ourexit", "ourls", "ourhistory", "ourpwd"};
int (*builtin_func[])(char **)={&shellcd, &shellhelp, &shellexit, &shellls, 
&shellhistory, &shellpwd};

int num_builtins(){ return sizeof(builtin_names)/sizeof(char*);}

int shellcd(char **args)
{
if (args[1]==NULL)
	printf("expected argument to cd");
else
	{
	int i=1;
	while(args[i])
	{	if(chdir(args[i])!=0)
		{
			perror("could not open path");
			break;
		}
		i++;
	}}
return 1;
}

int shellhelp(char **args)
{
int i;
printf("Our shell\nThe following commands are builtin:\n");
for (i=0; i<num_builtins(); i++)
	printf(" %s\n", builtin_names[i]);
printf("Use the man instead\n");
return 1;
}

int shellexit(char **args)
{
return 0;
}

int shellls(char **args)
{
char cwd[1024];
getcwd(cwd, sizeof(cwd));
DIR *d;
struct dirent *dir;
d=opendir(cwd);
if(d)
	{
	while((dir=readdir(d))!=NULL)
		printf("%s\n", dir->d_name);
	}
else
	{
	perror("could not open directory");
	return EXIT_FAILURE;
	}
return 1;
}

int shellpwd(char **args)
{
char cwd[1024];
getcwd(cwd, sizeof(cwd));
printf("%s\n", cwd);
return 1;
}


int shellhistory(char **args)
{
printf("\nOur history:\n");
printf("%s\n", istoric);
return 1;
}



#endif
