#include "pipe.h"

void	exec_cmds(t_pipe *pipex, int ac, char **av, char **env)
{
	t_pipe	*head;
	int	i;

	i = pipex->count_pipe;
	head = pipex;
	while (i > 0)
	{
		if (pipe(pipex->array->tab) < 0)
			write(1, "Error\n", 6);
		redir(pipex, env, pipex->array->tab[1]);
		pipex = pipex->next;
		--i;
	}
	redir(pipex, env, 3);
	wait_lst(head);
}

void    redir(t_pipe *pipex, char **env, int out)
{
	pipex->child = fork();
	if (pipex->child == 0)
	{
		if (out != 3)
		{
			verif_dup2(pipex->array->tab[1], 1);
			close(pipex->array->tab[1]);
		}
		close(pipex->array->tab[0]);
		exec(pipex, env);
	}
	else
	{
		if (out != 3)
		{
			verif_dup2(pipex->array->tab[0], 0);
			close(pipex->array->tab[0]);
		}
		close(pipex->array->tab[1]);
	}
}

void	exec(t_pipe *pipex, char **env)
{
	int		ret;
	char	*path;
	char	**args;

	ret = 0;
	path = get_cmd_opt(pipex->cmds[0], env);
	if (!path)	
	{
		puts("eror path");
		return ;
	}
	else
		execve(path, pipex->cmds, env);
}


void	wait_lst(t_pipe *pipex)
{
	while (pipex)
	{
		waitpid(pipex->child, NULL, 0);
		pipex = pipex->next;
	}
}
