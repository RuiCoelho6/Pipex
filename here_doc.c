/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:43:09 by rpires-c          #+#    #+#             */
/*   Updated: 2024/10/03 15:58:08 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_pipe_and_fork(int fd[2], pid_t *reader)
{
	if (pipe(fd) == -1)
		pipe_error();
	*reader = fork();
	if (*reader == -1)
	{
		close(fd[0]);
		close(fd[1]);
		fork_error();
	}
}

void	child_process(int fd[2], char *limiter)
{
	char	buffer[BUFFER_SIZE];
	size_t	bytes_read;

	close(fd[0]);
	bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		if (bytes_read >= ft_strlen(limiter)
			&& ft_strncmp(buffer, limiter, ft_strlen(limiter)) == 0
			&& (buffer[ft_strlen(limiter)] == '\n'
				|| buffer[ft_strlen(limiter)] == '\0'))
			break ;
		write(fd[1], buffer, bytes_read);
		bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE);
	}
	close(fd[1]);
	exit(EXIT_SUCCESS);
}

void	parent_process(int fd[2])
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	wait(NULL);
}

void	here_doc(char *limiter)
{
	int		fd[2];
	pid_t	reader;

	create_pipe_and_fork(fd, &reader);
	if (reader == 0)
		child_process(fd, limiter);
	else
		parent_process(fd);
}
