#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int exec(char **argc, char **envp, int i)
{
	1. üç tane değişken tanımla. (fd[2], status, int has_pipe);
	2. iki tane if tanımla (cd ve pipe icin)
	3. int pid = fork();
	4. if(!pid) (child prosess varsa)
		4.1 argv[i] = 0;
		4.2 has_pipe dup2(fd(0), 0) ve close fd 0 ve close fd1, -1 yapacaz ->error check
		4.3 *argv de cd var mi kontrol et
		4.4. execve(*argv, argv, envp);
		4.5 return(err("error: cannot execute"), err(*argv), err("\n"));

	5 waitpid(pid, &status, 0);
	6) dup2(fd[2],2) -close fd0 ve fd1
	7. return WIFEXICED(status) && WEXITSTATUS;
}

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