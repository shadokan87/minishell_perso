#ifndef _STRING_H
# define _STRING_H
#include "../gc/gc.h"
void	ft_putstr_fd(int fd, char *str);
char	*ft_strdup(char *str);
char	*ft_strjoin(char *s1, char *s2);
void	ft_putchar_str(char c, char **str);
int		ft_strlen(char *str);
int		get_c_index(char *s, char c);
char*	ft_strndup(char *str, int n);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_calloc(size_t count, size_t size);
char	**ft_split(char const *s, char c);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strrev(char *str);
#endif
