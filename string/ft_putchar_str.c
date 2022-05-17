#include "string.h"
#include <stdio.h>

void	ft_putchar_str(char c, char **str)
{
	char	*ptr;
	char	*tmp;

	ptr = *str;
	tmp = NULL;
	if (!ptr)
	{
		ptr = ft_strdup("c");
		if (!ptr)
			return ;
		ptr[0] = c;
		*str = ptr;
		return ;
	}
	tmp = ft_strjoin(*str, "c");
	tmp[ft_strlen(tmp) - 1] = c;
	*str = tmp;
}
