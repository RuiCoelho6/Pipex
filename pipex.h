/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:30:31 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/12 14:44:54 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "inc/libft/libft.h"

typedef struct s_btree
{
    char *cmd;
    struct s_btree *left;
    struct s_btree *right;
} t_btree;

void	fork_error(void);
void	pipe_error(void);
void	open_file_error(void);
void	command_error(void);
void	no_path_error(char *cmd);
char	*find_path(char *cmd, char **envp);
int		get_next_line(char **line);
void	execute(char *argv, char **envp);
int		open_file(char *argv, int i);
void	usage(void);

#endif
