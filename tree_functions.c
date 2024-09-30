/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 16:42:28 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/30 16:44:53 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
