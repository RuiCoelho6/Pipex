/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:31:46 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/19 20:45:42 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Funny tree builde

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

t_btree *create_node(char *cmd, t_btree *l, t_btree *r)
{
    t_btree *new_node = malloc(sizeof(t_btree));
    if (!new_node)
        return NULL;
    new_node->cmd = cmd;
    new_node->left = l;
    new_node->right = r;
    return new_node;
}

t_btree *build_tree(char **argv, int i, int end)
{
    t_btree *node;
    
    node = create_node(argv[i], NULL, NULL);
    if (i < end)
        node = create_node(NULL, node, build_tree(argv, i+1, end));
    return (node);
}

void process_tree(char **argv, t_btree *node, char **envp, int *is_first_command)
{
    int fd[2];
    pid_t pid;
    int filein;
    int fileout;

    if (node == NULL)
        return;
    if (pipe(fd) == -1)
        pipe_error();
    pid = fork();
    if (pid == -1)
        fork_error();
    if (pid == 0) 
    {
        if (node->left == NULL && node->right == NULL) 
        {
            if (*is_first_command)
            {
                filein = open_file(argv[1], 2); 
                if (filein == -1)
                    open_file_error();
                dup2(filein, STDIN_FILENO); 
                close(filein);         
                *is_first_command = 0; 
            }
            else
            {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[0]);
            }
            execute(node->cmd, envp);  
        }
        else 
        {
            close(fd[0]);              
            dup2(fd[1], STDOUT_FILENO);
            if (node->left)
                process_tree(argv, node->left, envp, is_first_command);
            if (node->right)
                process_tree(argv, node->right, envp, is_first_command);
        }
    }
    else 
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        waitpid(pid, NULL, 0);
        while (*argv)
            argv++;
        fileout = open_file(*(argv - 1), 1);
        if (fileout == -1)
            open_file_error();
        dup2(fileout, STDOUT_FILENO);
        close(fileout);
    }
}

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
		close(fd[0]);
		while (get_next_line(&line))
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
				exit(EXIT_SUCCESS);
			write(fd[1], line, ft_strlen(line));
		}
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
}

int main(int argc, char **argv, char **envp)
{
    t_btree *root;
    int first_cmd_flag;

    first_cmd_flag = 1;
    if (argc >= 5)
    {
        /* if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		{
			fileout = open_file(argv[argc - 1], 0);
			here_doc(argv[2], argc);
		}
        else */
        {
            root = build_tree(argv, 2, argc - 2);
            process_tree(argv, root, envp, &first_cmd_flag);
            execute(argv[argc - 2], envp);
        }
    }
    usage();
}
