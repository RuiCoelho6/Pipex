NAME = pipex
NAME_B = pipex_bonus

LIBFT = ./inc/libft/libft.a
INC = -I ./inc/libft
RM = rm -f

SRCS = pipex.c utils.c
OBJS = ${SRCS:.c=.o}

SRCS_B = pipex_bonus.c utils.c utils_bonus.c
OBJS_B = ${SRCS_B:.c=.o}

CC = cc -g
FLAGS = -Wall -Wextra -Werror

all: 		${NAME}

%.o : %.c
		@${CC} $(FLAGS) $(INC) -c $< -o $@

$(LIBFT):
	@make -C ./inc/libft

${NAME}:	${OBJS} ${LIBFT}
					@$(CC) ${FLAGS} ${OBJS} ${LIBFT} -o ${NAME}

bonus:		${NAME_B}

${NAME_B}:	${OBJS_B} ${LIBFT}
					@$(CC) ${FLAGS} ${OBJS_B} ${LIBFT} -o ${NAME_B}

clean:
					@make clean -C ./inc/libft
					@$(RM) $(OBJS) ${OBJS_B}

fclean: 	clean
					@make fclean -C ./inc/libft
					@$(RM) ${NAME} ${NAME_B}

re:			fclean all

re_bonus:	fclean bonus

.PHONY: all clean fclean re re_bonus bonus