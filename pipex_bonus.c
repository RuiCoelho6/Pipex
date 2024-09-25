/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:31:46 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/25 15:34:29 by rpires-c         ###   ########.fr       */
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

void    here_doc(char *limiter, int argc)
{
    pid_t   reader;
    int     fd[2];
    char    *line;

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
        close(fd[0]);
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

t_btree *create_node(char *cmd, t_btree *l, t_btree *r, int first_cmd)
{
    t_btree *new_node;

    new_node = malloc(sizeof(t_btree));
    if (!new_node)
        exit(EXIT_FAILURE);
    new_node->cmd = cmd;
    new_node->left = l;
    new_node->right = r;
    new_node->first_cmd = first_cmd;
    return (new_node);
}

t_btree *build_tree(char **argv, int i, int end)
{
    t_btree *node;

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

void    treat_left(char **argv, t_btree *node, char **envp, int *fd)
{
    int		infile;

	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout failed");
		exit(EXIT_FAILURE);
	}
	close(fd[1]);
	if (node->first_cmd == 1)
    {
        infile = open_file(argv[1], 2);
        if (dup2(fd[0], STDIN_FILENO) == -1)
        {
    	    perror("First file dup2 stdin failed");
            exit(EXIT_FAILURE);
        }
		close(infile);
    }
	close(fd[0]);
	execute(node->cmd, envp);
}

void process_tree(char **argv, t_btree *node, char **envp)
{
    int fd[2];
    pid_t pid1;
    int filein;
    int fileout;

	if (pipe(fd) == -1)
        pipe_error();
	pid1 = fork();
	if (pid1 == -1)
		fork_error();
	if(pid1 == 0)
		// Redirects stdin to infile and closes infile if it's the first node and closes fd[0]
		// Redirects stdout to fd[1] of the inherited pipe and closes fd[1]
		// Executes the command
		treat_left(argv, node->left, envp, fd);
	else
	{
		if (dup2(fd[0], STDIN_FILENO) == -1)
		{
			perror("dup2 stdin failed");
			exit(EXIT_FAILURE);
		}
		close(fd[0]);
		close(fd[1]);
		if (node->right->cmd == NULL)
			process_tree(argv, node->right, envp);
	}
}

int main(int argc, char **argv, char **envp)
{
    t_btree *root;

    if (argc >= 5)
    {
        root = build_tree(argv, 2, argc - 2);
        process_tree(argv, root, envp);
        free_tree(root);
    }
    else
        usage();
    return (0);
}
