/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:48:01 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/03 14:41:33 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cmd_type(char *cmd)
{
	if (!cmd)
		return (NOT_BUILT_IN_CMD);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0)
		return (BUILT_IN_CMD);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (EXIT_CMD);
	else
		return (NOT_BUILT_IN_CMD);
}

static char	*ft_join_process(char *name, char *value)
{
	char	*tmp;
	char	*result;

	if (!name)
		return (NULL);
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	if (value)
	{
		result = ft_strjoin(tmp, value);
		free(tmp);
		return (result);
	}
	else
		return (tmp);
}

char	**ft_env_to_array(t_env *env)
{
	t_env	*current;
	char	**env_array;
	int		i;

	current = env;
	i = 0;
	while (current && ++i)
		current = current->next;
	env_array = malloc(sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
	current = env;
	i = 0;
	while (current)
	{
		env_array[i++] = ft_join_process(current->name, current->value);
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

char	*ft_get_path_from_env(t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->name, "PATH") == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

char	*ft_get_path(char *cmd, t_env *env)
{
	char	*path;
	char	**dirs;
	char	*cmd_path;
	char	*temp;
	int		i;

	path = ft_get_path_from_env(env);
	if (!ft_strcmp(cmd, "") || !path)
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
			return (ft_free_array(&dirs), cmd_path);
		free(cmd_path);
		i++;
	}
	ft_free_array(&dirs);
	return (NULL);
}
