/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:43 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/22 17:19:14 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_env_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		i;

	current = env;
	i = 0;
	while (current)
	{
		i++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		env_array[i] = ft_strjoin(current->name, "=");
		env_array[i] = ft_strjoin(env_array[i], current->value);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

static void	ft_free_array(char **paths)
{
	int	i;

	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

static char	*ft_get_path_from_env(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->name, "PATH", 4) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static char	*ft_get_path(char *cmd, t_env *env)
{
	char	*path;
	char	**dirs;
	char	*cmd_path;
	char	*temp;
	int		i;

	path = ft_get_path_from_env(env);
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		temp = ft_strjoin(dirs[i], "/");
		cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(cmd_path, F_OK) == 0)
		{
			ft_free_array(dirs);
			return (cmd_path);
		}
		free(cmd_path);
		i++;
	}
	ft_free_array(dirs);
	return (NULL);
}

int	ft_execute_cmd(t_token *tokens, t_env *env)
{
	char	*path;
	char	**env_array;
	pid_t	pid;
	t_token	*current;
	int		i;
	char	**cmd;

	i = 0;
	current = tokens;
	env_array = ft_env_to_array(env);
	if (!ft_isbuilt_in(tokens->value, tokens, env))
	{
		path = ft_get_path(tokens->value, env);
		if (!path)
		{
			perror("Command not found");
			return (127);
		}
		while (current && current->type == WORD)
		{
			i++;
			current = current->next;
		}
		cmd = malloc(sizeof(char *) * (i + 1));
		if (!cmd)
			return (1);
		i = 0;
		while (tokens && tokens->type == WORD)
		{
			cmd[i++] = strdup(tokens->value);
			tokens = tokens->next;
		}
		cmd[i] = NULL;
		pid = fork();
		if (pid == 0)
		{
			execve(path, cmd, env_array);
			perror("execve");
			exit(1);
		}
		else if (pid < 0)
			perror("fork");
		wait(NULL);
		free(path);
		ft_free_array(env_array);
		ft_free_array(cmd);
	}
	return (0);
}

int	ft_execute(t_token *tokens, t_env *env)
{
	t_token	*current;

	current = tokens;
	if (!current)
		return (1);
	while (current)
	{
		if (current->type == WORD)
			return (ft_execute_cmd(current, env));
		current = current->next;
	}
	return (0);
}
