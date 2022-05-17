#include "pipe.h"

void	save_cmd_opt(t_pipe *pipex, char *str, int ac)
{
		char 	**array_cmd;
		t_pipe  *head;
		int		j;

		j = 0;
		while (j < ac - 1)
		{
			array_cmd = ft_split(str, 'k');
			if (!array_cmd[j])
				break ;
			j++;
		}
		head = pipex;
		j = 0;
		while (array_cmd[j])
		{
			head->cmds = ft_split(array_cmd[j], ' ');
			++j;
			head = head->next;
		}
}

char	*copy_cmd_shell(int ac, char **av)
{
	static	char	str[999999];
	int	o = 0;
	int	k = 0;
	int	j = 1;
	
	while (j < ac)
	{
		k = 0;
		while (av[j][k])
		{
			str[o] = av[j][k];
			++o;
			++k;
		}
		str[o++] = ' ';
		++j;
	}
	str[o] = '\0';
	return (str);
}

int	count_pipe(char *str)
{
	int	count;

	count = 0;
	while (str && *str)
	{
		if (*str == 'k')
			++count;
		++str;
	}
	return (count);
}

int main(int ac, char **av, char **env)
{
	char	*str;
	t_pipe  *pipex;
	t_mng	mng;
	int		i;
	int		count;

	if (ac < 2)
		write(1, "Number of arguments is wrong\n", 29);
	else if (is_empty(av) == 1)
		write(1, "Argument is empty\n", 29);
	else
	{
		i = 2;
		pipex = NULL;
		str = copy_cmd_shell(ac, av);
		count = count_pipe(str);
		pipex = create_lst(count, pipex, &mng);
		pipex->count_pipe = count;
		save_cmd_opt(pipex, str, ac);
		exec_cmds(pipex, ac, av, env);	
		free_lst(pipex);
	}
	return (0);
}
