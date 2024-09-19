/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:30:41 by rpires-c          #+#    #+#             */
/*   Updated: 2024/09/19 20:23:35 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_paths_from_env(char **envp)
{
	int	i;
	int	space_only;

	space_only = 1;
	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
	{
		if (!((*envp[i] >= 9 && *envp[i] <= 13) || *envp[i] == 32))
			space_only = 0;
		i++;
	}
	if (!envp[i] || space_only == 1)
		return (NULL);
	return pipex_split(envp[i] + 5, ':');
}

char	*build_and_check_path(char *path, char *cmd)
{
	char	*part_path;
	char	*full_path;

	part_path = ft_strjoin(path, "/");

	full_path = ft_strjoin(part_path, cmd);
	free(part_path);
	if (access(full_path, F_OK) == 0)
		return full_path;
	free(full_path);
	return NULL;
}

char *find_path(char *cmd, char **envp)
{
	char **paths;
	char *path;
	int i;

	paths = get_paths_from_env(envp);
	if(!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		path = build_and_check_path(paths[i], cmd);
		if (path)
		{
			while (paths[i])
				free(paths[i++]);
			free(paths);
			return path;
		}
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return NULL;
}


void	execute(char *argv, char **envp)
{
	char	**cmd;
	int		i;
	char	*path;

	i = -1;
	cmd = ft_split(argv, ' ');
	path = find_path(cmd[0], envp);
	if (!path)
	{
		while (cmd[++i])
			free(cmd[i]);
		free(cmd);
		no_path_error(argv);
	}
	if (execve(path, cmd, envp) == -1)
		command_error();
}
