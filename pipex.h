/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:30:31 by rpires-c          #+#    #+#             */
/*   Updated: 2024/10/03 15:43:52 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# define BUFFER_SIZE 10000

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "inc/libft/libft.h"

typedef struct s_btree
{
	char			*cmd;
	int				first_cmd;
	struct s_btree	*left;
	struct s_btree	*right;
}	t_btree;

void	fork_error(void);
void	pipe_error(void);
void	open_file_error(void);
void	command_error(void);
void	no_path_error(char *cmd);
char	*find_path(char *cmd, char **envp);
void	here_doc(char *limiter);
void	execute(char *argv, char **envp);
int		open_file(char *argv, int i);
char	**pipex_split(char const *s, char c);
void	process_tree(char **argv, int argc, t_btree *node, char **envp);
t_btree	*build_tree(char **argv, int i, int end);
t_btree	*create_node(char *cmd, t_btree *l, t_btree *r, int first_cmd);
void	usage(void);

#endif
