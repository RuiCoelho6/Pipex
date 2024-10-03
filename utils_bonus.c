/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:26:26 by rpires-c          #+#    #+#             */
/*   Updated: 2024/10/03 15:52:00 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	usage(void)
{
	ft_putstr_fd("Error: Bad arguments\n", 2);
	ft_putstr_fd("Ex: ./pipex_bonus <file1> <cmd1> <cmd2> <...> <file2>\n", 2);
	ft_putstr_fd("./pipex_bonus \"here_doc\" <LIMITER>", 2);
	ft_putstr_fd(" <cmd> <cmd1> <...> <file>\n", 2);
	exit(EXIT_SUCCESS);
}

int	open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (i == 2)
		file = open(argv, O_RDONLY, 0777);
	if (file == -1)
		open_file_error();
	return (file);
}
