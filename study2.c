#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int err(char *str)
{
	while(*str)
		write(2, str++, 1);
	return 1;
}

int cd(char **str, int i)
{
	//iki tane arg olmali. cunku subjecte diyor ki
	//Your program must implement the built-in command cd only with a path as argument
	if(i != 2)
		return (err("error: cd: bad arguments\n"));

	if(chdir(str[1]) == -1)
		return(err("error: cd: cannot change directory to "), err(str[1]), err("\n"));

	return 0;
}

int exe_cmd(char **argv, char **envp, int i)
{
	int fd[2];
	int status;
	int has_pipe = argv[i] && !strcmp(argv[i] , "|");

	if(!has_pipe && !strcmp(argv[i], "cd"))
		return(cd(argv, i));
	if(has_pipe && pipe(fd) == -1)
		return(err("error: error fatal\n"));

	int pid = fork();

	if(pid == 0)
	{
		while(argv[i] && argv[++i])
		{
			argv += i;
			i = 0;
			
		}
	}
}