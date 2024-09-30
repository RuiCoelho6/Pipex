/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:31:46 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/30 15:45:43 by rpires-c         ###   ########.fr       */
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

t_btree	*create_node(char *cmd, t_btree *l, t_btree *r, int first_cmd)
{
	t_btree	*new_node;

	new_node = malloc(sizeof(t_btree));
	if (!new_node)
		exit(EXIT_FAILURE);
	new_node->cmd = cmd;
	new_node->left = l;
	new_node->right = r;
	new_node->first_cmd = first_cmd;
	return (new_node);
}

t_btree	*build_tree(char **argv, int i, int end)
{
	t_btree	*node;

	if (i == 2)
		node = create_node(argv[i], NULL, NULL, 1);
	else if (i == end)
		node = create_node(argv[i], NULL, NULL, 2);
	else
		node = create_node(argv[i], NULL, NULL, 0);
	if (i < end)
	{
		node = create_node(NULL, node, build_tree(argv, i + 1, end), 0);
	}
	return (node);
}

void	process_tree(char **argv, int argc, t_btree *node, char **envp)
{
	int		fd[2];
	pid_t	pid;
	int		infile;
	int		outfile;

	if (node->cmd == NULL)
	{
		if (pipe(fd) == -1)
			pipe_error();
		pid = fork();
		if (pid == -1)
			fork_error();
		if (pid == 0)
		{
			close(fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			close(fd[1]);
			process_tree(argv, argc, node->left, envp);
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			process_tree(argv, argc, node->right, envp);
		}
	}
	else
	{
		if (node->first_cmd == 1)
		{
			infile = open_file(argv[1], 2);
			dup2(infile, STDIN_FILENO);
			close(infile);
		}
		else if (node->first_cmd == 2)
		{
			outfile = open_file(argv[argc - 1], 1);
			dup2(outfile, STDOUT_FILENO);
			close(outfile);
		}
		execute(node->cmd, envp);
	}
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
