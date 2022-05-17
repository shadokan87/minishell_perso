#include "string.h"

char*	ft_strndup(char *str, int n)
{
	int	i;
	char	*ret;

	i = 0;
	ret = gc_malloc(sizeof(char) * n + 1);
	while (str[i] && i < n)
	{
		ret[i] = str[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}
