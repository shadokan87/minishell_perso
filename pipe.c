#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>

int	main(int argc, char **argv)
{
	int fd[2];

	if (pipe(fd) == -1)
		return (1);
	int pid1 = fork();
	if (pid1 < 0)
		return (2);
	if (pid1 == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("ls", "ls", NULL);
	}
	int pid2 = fork();
	if (pid2 < 0)
		return (2);
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("grep", "grep", "file", NULL);
	}
	int pid3 = fork();
	if (pid3 < 0)
		return (2);
	if (pid3 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execlp("grep", "grep", "file", NULL);
	}

	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	waitpid(pid3, NULL, 0);
	return (0);
}
