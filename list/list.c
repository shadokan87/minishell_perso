#include "../minishell.h"

t_cut_cmd	*fill(char *elem, t_token __token__)
{
	t_cut_cmd	*ret;

	ret = gc_malloc(sizeof(t_cut_cmd));
	ret->is_last = 0;
	ret->wild_card_type = -1;
	ret->tail_wild_card = NULL;
	ret->head_wild_card = NULL;
	if (!elem)
		ret->elem = ft_strdup("");
	else
		ret->elem = ft_strdup(elem);
	ret->__token__ = __token__;
	ret->n = NULL;
	ret->p = NULL;
	ret->fd_flag = 0;
	return (ret);
}

int	add(t_msh *msh, char *elem, t_token __token__)
{
	t_cut_cmd	*ret;

	ret = fill(elem, __token__);
	#ifdef SHOW_ADD
	printf("[%s] -> [%d]\n", elem, __token__);
	#endif
	if (!msh->head)
	{
		msh->head = ret;
		msh->tail = msh->head;
		return (1);
	}
	ret->n = msh->head;
	msh->head->p = ret;
	msh->head = ret;
	return (1);
}
