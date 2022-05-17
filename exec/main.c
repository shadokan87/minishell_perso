#include "execute.h"

char	**get_path(char **env)
{
	char	**paths;

	while (ft_strncmp("PATH", *env, 4))
		++env;
	paths = ft_split(*env + 5, ':');
	return (paths);
}

char	*get_cmd_opt(char *cmd, char **env)
{
	char	**paths;
	char	*good_path;

	paths = get_path(env);
	while (*paths)
	{
		good_path = ft_strjoin_l(*paths, cmd);
		if (access(good_path, 0) == 0)
        	return (good_path);
        free(good_path);
		++paths;
	}
	return (NULL);
}

<<<<<<< HEAD
int main(int ac, char **av, char **env)
{
	int	i = 0;
    char    *path;
    char    *cmd[3];

=======
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


int main(int ac, char **av, char **env)
{
    char    *path;
	char	*cmd_shell;
	char	**cm;
>>>>>>> 6db5bc58d85de2719f5176fc25edd1660dbd3071
	if (ac < 2)
	{
		write(1, "Number of arguments is wrong\n", 29);
		exit(2);
	}
<<<<<<< HEAD
	while (i < ac)
	{
		cmd[i] = av[i + 1];
		++i;
	}
    cmd[i] = NULL;
    path = get_cmd_opt(av[1], env);
    if  (!execve(path, cmd, env))
=======
	cmd_shell = copy_cmd_shell(ac, av);
	cm = ft_split(cmd_shell, ' ');
    path = get_cmd_opt(cm[0], env);
    if  (!execve(path, cm, env))
>>>>>>> 6db5bc58d85de2719f5176fc25edd1660dbd3071
        write(1, "NULL\n", 5);
    return (0);
}