/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:31:46 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/30 16:43:31 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/* Funny tree builder

t_btree *build_tree(char **argv, int start, int end)
{
	int	mid;
	t_btree	*root;
	
	if (start > end)
		return NULL;
	mid = (start + end) / 2;
	root = create_node(argv[mid]);
	root->left = build_tree(argv, start, mid - 1);
	root->right = build_tree(argv, mid + 1, end);
	return root;
} */

void	here_doc(char *limiter, int argc)
{
	pid_t	reader;
	int		fd[2];
	char	*line;

	if (argc < 6)
		usage();
	if (pipe(fd) == -1)
		pipe_error();
	reader = fork();
	if (reader == -1)
	{
		fork_error();
	}
	if (reader == 0)
	{
		close (fd[0]);
		while (get_next_line(&line))
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
				&& ft_strlen(line) == ft_strlen(limiter))
			{
				free(line);
				exit(EXIT_SUCCESS);
			}
			write(fd[1], line, ft_strlen(line));
			free(line);
		}
		close(fd[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		wait(NULL);
	}
}

void	free_tree(t_btree *node)
{
	if (node == NULL)
		return ;
	free_tree(node->left);
	free_tree(node->right);
	free(node);
}

int	main(int argc, char **argv, char **envp)
{
	t_btree	*root;
	pid_t	pid;

	if (argc >= 5)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			here_doc(argv[2], argc);
			root = build_tree(argv, 3, argc - 2);
		}
		else
			root = build_tree(argv, 2, argc - 2);
		pid = fork();
		if (pid == 0)
			process_tree(argv, argc, root, envp);
		waitpid(pid, NULL, 0);
		free_tree(root);
	}
	else
		usage();
	return (0);
}
