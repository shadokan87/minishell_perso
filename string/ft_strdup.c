#include "string.h"

char*	ft_strdup(char *str)
{
	return (ft_strndup(str, ft_strlen(str)));
}
