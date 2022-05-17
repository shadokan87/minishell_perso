#ifndef MINISHELL_H
#define MINISHELL_H
#include <unistd.h>
#include <stdlib.h>
#include "string/string.h"
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#define PROMPT_NAME "minishell% "
#define ERROR_HEADER "minishell: "
#define HISTORY_PATH "~/msh_history"
/* #define SHOW_ADD */
#define SYMBOLS ">:<:|:&"
#define SYMBOLS_LOOKUP "|:||:<:<<:>:>>:&&"

typedef enum __token__
{
	_START,
	_UNASSIGNED,
	C_BUILTIN,
	C_ENV,
	LAST_CMD,
	ARG,
	OPTION,
	S_QUOTE,
	D_QUOTE,
	WILD_CARD,
	ENV_SOLO,
	_SYMBOL_START,
	PIPE,
	R_REDIR,
	D_R_REDIR,
	L_REDIR,
	D_L_REDIR,
	OR,
	AND,
	_SYMBOL_END,
	_END,
}	t_token;

typedef struct s_cut_cmd
{
	t_token				__token__;
	char				*elem;
	int					is_last;
	int					wild_card_type;
	int					fd_flag;
	int					pid;
	struct s_cut_cmd	*n;
	struct s_cut_cmd	*p;
	struct s_cut_cmd	*tail_wild_card;
	struct s_cut_cmd	*head_wild_card;
}	t_cut_cmd;

typedef struct s_msh
{
	char	*line;
	char	*parsing_error;
	char	*execution_error;
	int		waitforparse;
	int		exit_code;
	int		single_command;
	int		run_status;
	int		history_fd;
	t_cut_cmd	*head;
	t_cut_cmd	*tail;
	t_cut_cmd	*env_head;
	t_cut_cmd	*env_tail;
}	t_msh;

t_cut_cmd	*fill(char *elem, t_token __token__);
int	add(t_msh *msh, char *elem, t_token __token__);

# endif
