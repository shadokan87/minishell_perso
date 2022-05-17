#include "string.h"

int	get_c_index(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	if (c == '\0')
	{
		while (str[i])
			i++;
		return (i);
	}
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}
