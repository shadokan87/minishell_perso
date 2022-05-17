#include "string.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!dst || !src)
		return (0);
	while (src[i] && dstsize > 1)
	{
		dst[i] = src[i];
		dstsize--;
		i++;
	}
	if (dstsize > 0)
		dst[i] = '\0';
	while (src[i])
		i++;
	return (i);
}

int	check_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

int	len_trim(char *s1, const char *set)
{
	int	i;
	int	y;

	y = 0;
	i = 0;
	while (check_set(s1[i], set) && s1[i])
		i++;
	if (s1[i] == '\0')
		return (1);
	y = i;
	i = ft_strlen((char *)s1) - 1;
	while (check_set(s1[i], set))
		i--;
	return (i - y + 1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	int		len;
	char	*s2;

	if (!s1 || !set)
		return (ft_strdup(""));
	s2 = (char *)s1;
	len = len_trim(s2, set);
	str = gc_malloc(sizeof(char) * len + 1);
	if (!str)
		return (0);
	while (check_set(*s2, set))
		s2++;
	ft_strlcpy(str, s2, len + 1);
	return (str);
}
