#include "string.h"

char	*ft_strrev(char *str)
{
	int		i;
	int		y;
	char	swap;

	swap = '0';
	i = 0;
	y = ft_strlen(str) - 1;
	while (i < y)
	{
		swap = str[i];
		str[i] = str[y];
		str[y] = swap;
		i++;
		y--;
	}
	return (str);
}
