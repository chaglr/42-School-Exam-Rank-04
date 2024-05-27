#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int exec(char **argv ,char **envp, int i)
{
	int fd[2];
	int status;
	int has_pipe = argv[i] && !strcmp(argv[i], "|");

	if(!has_pipe && !strcmp(argv[i], "cd"))
		return(cd(argv, i));

	if(has_pipe && pipe(fd) == -1)
		return(err("error: fatal\n"));

	int pid = fork();

	if(pid == 0)
	{
		argv[i] = 0;
	}
}