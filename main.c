#include "minishell.h"

t_gc base;

void	throw_error(char *msg)
{
	ft_putstr_fd(2, ERROR_HEADER);
	ft_putstr_fd(2, msg);
	ft_putstr_fd(2, "\n");
}

void	throw_error_fatal(char *msg, int code)
{
	ft_putstr_fd(2, ERROR_HEADER);
	ft_putstr_fd(2, msg);
	ft_putstr_fd(2, "\n");
	exit(code);
}

//returns len until find is met, set found to iterator position
int		symbol_len(t_cut_cmd* pos, enum __token__ find, t_cut_cmd** found)
{
	t_cut_cmd*	iterator;
	int			len;

	iterator = pos;
	len = 0;
	while (iterator && iterator->__token__ != find)
	{
		len++;
		iterator = iterator->p;
	}
	*found = iterator;
	return (len);
}

//returns 1 or 0 wheter C is present or not
int		c_is_present(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

//returns len of a list from pos to end token
int		list_len(t_cut_cmd* pos, enum __token__ end)
{
	t_cut_cmd*	iterator;
	int			len;

	iterator = pos;
	len = 0;
	while (iterator && iterator->__token__ != end)
	{
		len++;
		iterator = iterator->p;
	}
	return (len);
}

//convert a list to a split (char**)
char	**list_to_split(t_cut_cmd *target)
{
	char		**ret;
	int			i;

	i = 0;
	ret = ft_calloc(list_len(target, _END) + 1, sizeof(char *));
	while (target)
	{
		ret[i] = ft_strdup(target->elem);
		target = target->p;
		i++;
	}
	return (ret);
}

//returns pointer to var env node
t_cut_cmd*	get_env_ptr(char *var)
{
	t_cut_cmd*	iterator;
	t_msh*		msh;
	char		*ret;
	int			i;

	i = 0;
	msh = (t_msh*)base.prog;
	iterator = msh->env_tail;
	ret = NULL;
	while (iterator)
	{
		while (iterator->elem[i] && iterator->elem[i] != '=')
		{
			ft_putchar_str(iterator->elem[i], &ret);
			i++;
		}
		if (is_same(ret, var))
			return (iterator);
		iterator = iterator->p;
		ret = NULL;
		i = 0;
	}
	return (NULL);
}

//returns value of env node
char	*ft_get_env(t_cut_cmd *env)
{
	char **split;

	if (!env)
		return (NULL);
	split = ft_split(env->elem, '=');
	if (!split[1])
		return (split[0]);
	return (split[1]);
}

//returns path of a command
char	*get_path(char *cmd)
{
	char		*path;
	char		**paths;
	char		*good_path;

	path = ft_get_env(get_env_ptr("PATH"));
	if (!path || c_is_present(cmd, '/'))
		return (cmd);
	paths = ft_split(path, ':');
	while (*paths)
	{
		good_path  = ft_strjoin(*paths, ft_strjoin("/", cmd));
		if (access(good_path, 0) == 0)
			return (good_path);
		++paths;
	}
	return (NULL);
}

//backup / restore msh->head and msh->tail
t_cut_cmd	**head_tail(t_msh *msh, char *mode)
{
	static t_cut_cmd	**bak;

	if (!bak)
	{
		gc("pause");
		bak = gc_malloc(sizeof(t_cut_cmd *) * 2);
		gc("resume");
	}
	if (is_same(mode, "backup") && !bak[0] && !bak[1])
	{
		bak[0] = msh->head;
		bak[1] = msh->tail;
		msh->head = NULL;
		msh->tail = NULL;
	}
	else if (is_same(mode, "restore"))
	{
		msh->head = bak[0];
		msh->tail = bak[1];
		bak = NULL;
	}
	return (bak);
}

//returns current working directory
char	*get_cwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)))
		return (ft_strdup(cwd));
	return (NULL);
}

//returns current directory from cwd
char*	get_prompt_of(t_msh* msh, char *cwd)
{
	char	*ret;
	char	*home;
	(void)ret;
	(void)msh;
	home = ft_get_env(get_env_ptr("HOME"));
	if (!home)
		return (PROMPT_NAME);
	if (is_same(home, cwd))
		return (ft_strdup("~ "));
	if (is_same(cwd, "/"))
		return (ft_strdup("/ "));
	cwd = ft_strrev(cwd);
	ret = ft_strndup(cwd, get_c_index(cwd, '/'));
	return (ft_strjoin(ft_strrev(ret), " "));
}

char	*get_line(char *prompt_name)
{
	char	*ret;
	char	*curr_path;
	t_msh*	msh;

	(void)prompt_name;
	msh = (t_msh*)base.prog;
	curr_path = get_prompt_of(msh, get_cwd());
	ret = readline(curr_path);
	return (ret);
}

void	opt_single_command(t_msh *msh, char *cmd)
{
	msh->single_command = 1;
	msh->line = ft_strdup(cmd);
}

void	get_options(t_msh* msh, int argc, char **argv)
{
	int	i;

	i = 0;
	while (i < argc)
	{
		if (is_same(argv[i], "-c") && argv[i + 1])
			opt_single_command(msh, argv[i + 1]);
		i++;
	}
}

void	get_history_from_fd(int fd)
{
	char	buff[2];
	char	*ret;

	ret = NULL;
	while (read(fd, &buff, 1))
	{
		buff[1] = '\0';
		if (!ret)
			ret = ft_strdup(buff);
		else if (buff[0] == '\n')
		{
			add_history(ret);
			ret = NULL;
		}
		else
			ret = ft_strjoin(ret, buff);
	}
	if (ret)
		add_history(ret);
}

void	init_history(t_msh* msh)
{
	char*	home;

	home = ft_get_env(get_env_ptr("HOME"));
	if (!home)
	{
		throw_error("failed to get home path");
		msh->history_fd = open("/tmp/.msh_history", O_RDWR | O_APPEND | O_CREAT, 0666);
	}
	else
		msh->history_fd = open(ft_strjoin(home, "/.msh_history"), O_RDWR | O_APPEND | O_CREAT, 0666);
	if (msh->history_fd == -1)
	{
		throw_error(ft_strjoin("could not load history from: ", HISTORY_PATH));
		return ;
	}
	get_history_from_fd(msh->history_fd);
}

void	init_env(t_msh* msh, char **envp)
{
	(void)msh;
	(void)envp;
	int		i;

	i = 0;
	if (!envp)
		return ;
	gc("pause");
	while (envp[i])
	{
		add(msh, envp[i], _UNASSIGNED);
		i++;
	}
	gc("resume");
	msh->env_head = msh->head;
	msh->env_tail = msh->tail;
	msh->head = NULL;
	msh->tail = NULL;
}

void	init(int argc, char **argv, char **envp)
{
	t_msh* msh;
	(void)envp;

	gc_init();
	gc("pause");
	base.prog = (void*)gc_malloc(sizeof(t_msh));
	gc("resume");
	msh = (t_msh*)base.prog;
	msh->waitforparse = 1;
	msh->line = NULL;
	msh->head = NULL;
	msh->tail = NULL;
	msh->env_head = NULL;
	msh->env_tail = NULL;
	msh->parsing_error = NULL;
	msh->execution_error = NULL;
	msh->exit_code = 0;
	msh->run_status = 1;
	msh->single_command = 0;
	get_options(msh, argc, argv);
	init_env(msh, envp);
	init_history(msh);
}

t_token	determine_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (WILD_CARD);
		i++;
	}
	return (_UNASSIGNED);
}

int		is_symbol(char c)
{
	static char **_symbols;
	int			i;

	i = 0;
	if (!_symbols)
	{
		gc("pause");
		_symbols = ft_split(SYMBOLS, ':');
		gc("resume");
	}
	while (_symbols[i] && _symbols[i][0] != c)
		i++;
	if (_symbols[i])
		return (i);
	return (-1);
}

t_token parsing_error_near(char *str)
{
	t_msh*	msh;
	char	c;
	int		error_type;
	int		i;

	i = 0;
	msh = (t_msh*)base.prog;
	msh->waitforparse = -1;
	msh->parsing_error = ft_strdup("syntax error near unexpected token '");
	error_type = (ft_strlen(str) >= 3);
	if (error_type)
		msh->parsing_error = ft_strjoin(msh->parsing_error, "xx'");
	else
		msh->parsing_error = ft_strjoin(msh->parsing_error, "x'");
	c = str[0];
	while (str[i] && str[i] == c && !(i > 2))
		i++;
	if (str[i])
		msh->parsing_error[ft_strlen(msh->parsing_error) - 2] = str[i];
	else
		msh->parsing_error[ft_strlen(msh->parsing_error) - 2] = c;
	if (error_type)
		msh->parsing_error[ft_strlen(msh->parsing_error) - 3] = str[i - 1];
	return  (_UNASSIGNED);
}

t_token	determine_token(char *str)
{
	if (str[0] == '\"')
		return (D_QUOTE);
	if (str[0] == '\'')
		return (S_QUOTE);
	if (is_same(str, "|"))
		return (PIPE);
	if (is_same(str, "||"))
		return (OR);
	if (is_same(str, "&&"))
		return (AND);
	if (is_same(str, ">"))
		return (R_REDIR);
	if (is_same(str, ">>"))
		return (D_R_REDIR);
	if (is_same(str, "<"))
		return (L_REDIR);
	if (is_same(str, "<<"))
		return (D_L_REDIR);
	if (is_symbol(str[0]) != -1)
		return (parsing_error_near(str));
	return (determine_wildcard(str));
}

void	case_delimitor(t_msh *msh, char **s)
{
	add(msh, *s, determine_token(*s));
	*s = NULL;
}

int		end_of_quote(int i, int mark_double, int mark_single, char *s)
{
	(void)i;
	(void)mark_double;
	(void)mark_single;
	(void)s;
	if (mark_double != 0 && mark_double % 2 == 0)
		return (1);
	if (mark_single != 0 && mark_single % 2 == 0)
		return (1);
	return (0);
}

void	case_quote_error(t_msh *msh)
{
	msh->parsing_error = ft_strdup("dquote feature not supported");
	msh->waitforparse = -1;
}

void	case_quote_append_to_list(t_msh* msh, char *ret, char *s)
{
	(void)s;
	add(msh, ret, determine_token(ret));
}

int		case_quote(t_msh* msh, int i, char **s)
{
	int		mark_double;
	int		mark_single;
	char	*str;
	char	*ret;

	mark_double = 0;
	mark_single = 0;
	str = *s;
	ret = NULL;
	while (str[i])
	{
		if (end_of_quote(i, mark_double, mark_single, str))
			break ;
		if (str[i] == '\"' && mark_single % 2 == 0)
			mark_double++;
		if (str[i] == '\'' && mark_double % 2 == 0)
			mark_single++;
		ft_putchar_str(str[i], &ret);
		i++;
	}
	if (!end_of_quote(i, mark_double, mark_single, str))
		case_quote_error(msh);
	if (ret)
		add(msh, ret, determine_token(ret));
	return (i - 1);
}

void	cut_line(t_msh *msh)
{
	char	*str;
	char	*s;
	int		i;

	i = 0;
	str = msh->line;
	s = NULL;
	while (str[i])
	{
		if (str[i] == ' ' && s)
			case_delimitor(msh, &s);
		else if (str[i] != ' ' && (str[i] != '\'' && str[i] != '\"'))
		{
			if ((is_symbol(str[i]) != -1 && s && is_symbol(s[0]) == -1)
				|| (is_symbol(str[i]) == -1 && s && is_symbol(s[0]) != -1))
				case_delimitor(msh, &s);
			ft_putchar_str(str[i], &s);
		}
		else if (str[i] == '\'' || str[i] == '\"')
			i = case_quote(msh, i, &str);
		i++;
	}
	if (s)
		add(msh, s, determine_token(s));
	if (msh->waitforparse != -1)
		msh->waitforparse = 0;
}

void	set_tokens(t_msh *msh)
{
	(void)msh;
}

t_cut_cmd* get_next_symbol(t_cut_cmd* pos)
{
	t_cut_cmd*	iterator;

	iterator = pos;
	while (iterator)
	{
		if (iterator->__token__ > _SYMBOL_START)
			return (iterator);
		iterator = iterator->p;
	}
	return (iterator);
}

char	*remove_quotes(char* str)
{
	char	c;
	char	*ret;
	int		i;

	c = str[0];
	ret = NULL;
	i = 0;
	while (str[i])
	{
		if (str[i] != c)
			ft_putchar_str(str[i], &ret);
		i++;
	}
	return (ret);
}

void	check_linked_list_format(t_msh* msh)
{
	t_cut_cmd* iterator;

	iterator = msh->tail;
	if (!msh->tail || msh->parsing_error  < 0)
		return ;
	while (!(!iterator->p))
	{
		iterator = iterator->p;
	}
	if (iterator->__token__ > _SYMBOL_START)
	{
		msh->parsing_error = ft_strdup("syntax error near unexpected token 'newline'");
		msh->waitforparse = -1;
	}
	iterator = msh->tail;
	while (iterator)
	{
		if (iterator->__token__ == D_QUOTE || iterator->__token__ == S_QUOTE)
			iterator->elem = remove_quotes(iterator->elem);
		iterator = iterator->p;
	}
}

int	parse()
{
	t_msh *msh;

	msh = (t_msh*)base.prog;
	if (!msh->line || is_same(ft_strtrim(msh->line, " "), ""))
		msh->waitforparse = -2;
	while (msh->waitforparse > 0)
	{
		cut_line(msh);
		check_linked_list_format(msh);
		set_tokens(msh);
	}
	if (msh->waitforparse < 0)
	{
		if (msh->waitforparse != -2)
		{
			ft_putstr_fd(2, ERROR_HEADER);
			ft_putstr_fd(2, msh->parsing_error);
			ft_putstr_fd(2, "\n");
		}
		return (0);
	}
	return (1);
}

int	ft_readdir(struct dirent **de, DIR *dr)
{
	*de = readdir(dr);
	return ((*de != NULL));
}

t_cut_cmd	*ft_ls(char *path, char *options)
{
	struct dirent	*de;
	DIR				*dr;
	char			*tmp;
	t_cut_cmd		*concat;

	(void)concat;
	if (path)
		dr = opendir(path);
	else
		dr = opendir(".");
	concat = NULL;
	while (dr != NULL && ft_readdir(&de, dr))
	{
		tmp = ft_strdup(de->d_name);
		if (!is_same(options, "-a") && tmp[0] == '.')
			continue ;
		/* else */
			/* add_to_env(&concat, tmp, _UNASSIGNED); */
		tmp = NULL;
	}
	if (dr)
		closedir(dr);
	return (concat);
}

void	execute_fork_error()
{
	ft_putstr_fd(2, "fork failed exiting now.");
	exit(255);
}

int	next_token_is_pipe(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (is_same(argv[i], "&&") || is_same(argv[i], "||"))
			return (0);
		if (is_same(argv[i], "|"))
			return (1);
		i++;
	}
	return (0);
}

int	get_next_symbol_av(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		if (is_symbol(argv[i][0]) != -1)
			return (i);
		i++;
	}
	return (i);
}
#define $BR(x) ft_putstr_fd(2, x);

char	**env_to_split(t_msh* msh)
{
	if (!msh->tail)
		return (NULL);
	return (list_to_split(msh->env_tail));
}

int		command_not_found(char *str)
{
	t_msh* msh;

	msh = (t_msh*)base.prog;
	msh->exit_code = 127;
	ft_putstr_fd(2, ft_strjoin(ft_strjoin(ERROR_HEADER,
				"command not found: "), str));
	ft_putstr_fd(2, "\n");
	return (-1);
}

void	handle_pipe(char **argv)
{
	(void)argv;
}

void	move_pos(t_msh* msh, t_cut_cmd* pos)
{
	pos = get_next_symbol(pos);
	if (pos && pos->__token__ == AND && msh->exit_code)
		pos = NULL;
	else if (pos && pos->__token__ == OR && !msh->exit_code)
		pos = NULL;
	else if (pos)
		pos = pos->p;
	msh->tail = pos;
}

int		execute()
{
	t_cut_cmd	*iterator;
	t_msh* msh;
	char**	av;
	int	i;

	msh = (t_msh*)base.prog;
	i = 0;
	iterator = msh->tail;
	av = list_to_split(iterator);
	while (av[i])
	{
		printf("[%s]\n", av[i]);
		av[0] = get_path(av[0]);
		i++;
	}
	int prev_pipe = 0;

	i = 0;
	while (av[i])
	{
		if (!av[i])
			return (0);
		/* if (strcmp(av[i], "cd") == 0) */
		/* { */
		/* 	i += exec_cd(av + i); */
		/* } */
		else
		{
			exec_cmd(av + i, list_to_split(msh->env_tail), &prev_pipe);
			i += get_next_symbol_av(av + i);
		}
		if (av[i])
			i++;
	}
	return (0);
	/* t_cut_cmd	*iterator; */
	/* t_msh		*msh; */
	/* int			argc; */
	/* char**		argv; */
	/* char		*tmp; */

	/* msh = (t_msh*)base.prog; */
	/* if (!msh->tail) */
	/* { */
	/* 	add_history(msh->line); */
	/* 	ft_putstr_fd(msh->history_fd, ft_strjoin(msh->line, "\n")); */
	/* 	return (0); */
	/* } */
	/* iterator = msh->tail; */
	/* argc = list_len(iterator, _END); */
	/* argv = list_to_split(iterator); */
	/* tmp = ft_strdup(argv[0]); */
	/* argv[0] = get_path(argv[0]); */
	/* if (!argv[0]) */
	/* 	command_not_found(tmp); */
	/* execute_one(argv, argc, env_to_split(msh)); */
	/* move_pos(msh, msh->tail); */
	/* return (execute()); */
}

void	clear_cycle(t_msh *msh)
{
	msh->head = NULL;
	msh->tail = NULL;
	msh->waitforparse = 1;
	if (msh->single_command)
		msh->run_status = 0;
	gc("collect");
}

int	main(int argc, char **argv, char **envp)
{
	t_msh *msh;
	(void)argc;
	(void)argv;
	(void)envp;
	init(argc, argv, envp);
	msh = (t_msh*)base.prog;
	while (msh->run_status)
	{
		if (!msh->single_command)
			msh->line = get_line(PROMPT_NAME);
		if (parse())
			execute();
		clear_cycle(msh);
	}
	gc("collect -all");
	return (0);
}
