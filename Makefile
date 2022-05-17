NAME	=	minishell

CC		=	clang

RM		=	rm -f

CFLAGS	=	-Wall -Wextra -Werror -g3 -fsanitize=address

LFLAGS	=	-I. -lncurses -lreadline -L /usr/local/Cellar/readline/8.1/lib -I /usr/local/Cellar/readline/8.1/include

SRCS	=	./main.c \
			string/ft_putchar_str.c \
			string/ft_putstr_fd.c   \
			string/ft_strdup.c      \
			string/ft_strndup.c      \
			string/ft_strjoin.c     \
			string/ft_strlen.c      \
			string/is_same.c \
			string/ft_calloc.c \
			string/ft_memset.c \
			string/ft_split.c \
			string/ft_strtrim.c \
			string/ft_strrev.c \
			string/get_c_index.c \
			gc/garbage_co.c \
			gc/garbage_co2.c \
			gc/garbage_co3.c \
			list/list.c

OBJS	=	$(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			${CC} ${CFLAGS} -lreadline ${LFLAGS} -o ${NAME} ${OBJS}

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME) $(LIBFT)

re:			fclean $(NAME)

f:			re
			./minishell

.PHONY:		all clean fclean re f
