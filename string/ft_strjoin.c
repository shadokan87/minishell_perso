#include "string.h"

char*	ft_strjoin(char *s1, char *s2)
{
	char	*ret;
	int		i;
	int		y;
	int		len;

	i = 0;
	y = 0;
	len = ft_strlen(s1) + ft_strlen(s2);
	ret = malloc(sizeof(char) * len + 1);
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	while (s2[y])
	{
		ret[i + y] = s2[y];
		y++;
	}
	ret[i + y] = '\0';
	return (ret);
}
