/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_co3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tidminta <tidminta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/21 20:55:37 by tidminta          #+#    #+#             */
/*   Updated: 2021/09/21 20:55:45 by tidminta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gc.h"

void	gc_free_calloc(char **_calloc)
{
	int	i;

	i = -1;
	while (_calloc[++i])
		gc_free(_calloc[i]);
	gc_free(_calloc[i]);
	gc_free(_calloc);
}

void	gc_free(void *ptr)
{
	if (!free_mute_target(ptr))
		free_normal_target(ptr);
}

int	push_to_normal(t_addr *_new)
{
	if (g_gc && !g_gc->normal)
	{
		g_gc->normal = _new;
		return (1);
	}
	if (g_gc && g_gc->normal)
	{
		_new->n = g_gc->normal;
		g_gc->normal->p = _new;
		g_gc->normal = _new;
		return (1);
	}
	return (1);
}

int	gc_push(void ***g)
{
	t_addr	*_new;

	_new = malloc(sizeof(t_addr));
	if (!_new)
		return (0);
	_new->g = g;
	_new->n = NULL;
	_new->p = NULL;
	if (g_gc && g_gc->dst)
		return (push_to_mute(_new));
	else
		return (push_to_normal(_new));
	return (1);
}
