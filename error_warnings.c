/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_warnings.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 15:50:27 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/22 20:52:40 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fork_error(void)
{
	perror("Error creating child process");
	exit(EXIT_FAILURE);
}

void	pipe_error(void)
{
	perror("Error creating the pipe");
	exit(EXIT_FAILURE);
}

void	open_file_error(void)
{
	perror("Error opening the file");
	exit(EXIT_FAILURE);
}

void	no_path_error(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		write(2, &cmd[i], 1);
		i++;
	}
	write(2, ": command not found\n", 21);
	exit(EXIT_FAILURE);
}

void	command_error(void)
{
	perror("Error executing the command");
	exit(EXIT_FAILURE);
}
