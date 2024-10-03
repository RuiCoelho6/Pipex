/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:31:46 by rpires-c          #+#    #+#             */
/*   Updated: 2024/10/03 15:44:56 by rpires-c         ###   ########.fr       */
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
			here_doc(argv[2]);
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
