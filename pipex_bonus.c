/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rpires-c <rpires-c@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 15:31:46 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/24 11:44:18 by rpires-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*
Funny tree builder

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

void process_tree(char **argv, t_btree *node, char **envp)
{
    int fd[2];
    pid_t pid;
    int filein;
    int fileout;

    printf("\n");
    if (pipe(fd) == -1)
        pipe_error();
        
    pid = fork();
    if (pid == -1)
        fork_error();
    if (pid == 0)
    {
        printf("Child:  command: %s | number: %d\n", node->cmd, node->first_cmd);
        if (node->cmd != NULL)
        {
            if (node->left == NULL && node->right == NULL)
            {
                // Treating from where to input
                if (node->first_cmd == 1)
                {
                    filein = open_file(argv[1], 2);
                    printf("Child:  opening input file: %s | input file fd: %d\n", argv[1], filein);
                    if (dup2(filein, STDIN_FILENO) ==  -1)
                    {
                        perror("dup2 stdin failed");
                        exit(EXIT_FAILURE);
                    }
                    printf("Child:  redirecting stdin to input file fd: %d\n", filein);
                    close(filein);
                    printf("Child:  closing input file: %s | input file fd: %d\n", argv[1], filein);
                }
                else
                {
                    if (dup2(fd[0], STDIN_FILENO) == -1)
                    {
                        perror("dup2 stdin failed");
                        exit(EXIT_FAILURE);
                    }
                    printf("Child:  redirecting stdin to pipe fd[0]\n");
                    close(fd[0]);
                    close(fd[1]);
                    printf("Child:  closing fd[0] and fd[1]\n");
                }
                // Treating for where to output
                if (node->first_cmd == 2)
                {
                    while (*argv)
                        argv++;
                    fileout = open_file(*(argv - 1), 1);
                    printf("Child:  opening output file: %s | output file fd: %d\n", *(argv - 1), fileout);
                    if (dup2(fileout, STDOUT_FILENO) == -1)
                    {
                        perror("dup2 stdin failed");
                        exit(EXIT_FAILURE);
                    }
                    printf("Child:  redirecting stdout to output file fd: %d\n", fileout);
                    close(fileout);
                    printf("Child:  closing output file: %s | output file fd: %d\n", *(argv - 1), fileout);
                }
                else
                {
                    if (dup2(fd[1], STDOUT_FILENO) == -1)
                    {
                        perror("dup2 stdout failed");
                        exit(EXIT_FAILURE);
                    }
                    close(fd[1]);
                    printf("Child:  redirecting stdout to pipe: fd[1]\n");
                }
                printf("Child:  executing command: %s\n", node->cmd);
                execute(node->cmd, envp);
            }
        }
        else
        {
            printf("Child:  processing subtree block\n");
            if (node->left)
            {
                printf("Child:  processing left subtree\n");
                process_tree(argv, node->left, envp);
            }
            if (node->right)
            {
                printf("Child:  processing right subtree\n");
                process_tree(argv, node->right, envp);
            }
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(fd[0]);
        close(fd[1]);
        printf("Parent: closing both pipe ends\n");
        printf("Parent: waiting for child\n");
        waitpid(pid, NULL, 0);
        printf("Parent: child process finished\n");
    }
}

void print_tree(t_btree *node)
{
    if (node == NULL)
        return;

    printf("Node cmd: %s\n", node->cmd ? node->cmd : "(null)");
    printf("First command flag: %d\n", node->first_cmd);
    printf("Left child: %s\n", (node->left && node->left->cmd) ? node->left->cmd : "(null)");
    printf("Right child: %s\n", (node->right && node->right->cmd) ? node->right->cmd : "(null)");
    printf("\n");

    print_tree(node->left);
    print_tree(node->right);
}

void free_tree(t_btree *node)
{
    if (node == NULL)
        return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}

int main(int argc, char **argv, char **envp)
{
    t_btree *root;

    if (argc >= 5)
    {
        root = build_tree(argv, 2, argc - 2);
        print_tree(root);
        printf("--------------------------------\n");
        process_tree(argv, root, envp);
        free_tree(root);
    }
    else
        usage();
    return (0);
}
