MANDA_NAMES = 	../libft/ft_split.c ../libft/ft_strlen.c ../libft/ft_strncmp.c\
				../libft/ft_calloc.c ../libft/ft_memset.c ../libft/ft_memcpy.c\
				main.c utils.c

MANDA_OBJS =	 $(MANDA_NAMES:.c=.o)

<<<<<<< HEAD
NAME = pipex
=======
NAME = exec
>>>>>>> 6db5bc58d85de2719f5176fc25edd1660dbd3071

CC = clang
CFLAGS = -Wall -Wextra -Werror

AR = ar rc

RM = rm -f

RL = ranlib

$(NAME):	$(MANDA_OBJS)
			$(CC) $(CFLAGS) $(MANDA_OBJS) -o $(NAME)

all:		$(NAME)

clean:	
			$(RM) $(MANDA_OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all bonus clean fclean re
