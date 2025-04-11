/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 09:48:01 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/10 20:46:45 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_join_process(char *name, char *value)
{
	char	*tmp;
	char	*result;

	if (!name || !value)
		return (NULL);
	tmp = ft_strjoin(name, "=");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, value);
	free(tmp);
	return (result);
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

void	ft_free_array(char ***paths)
{
	int	i;

	i = 0;
	if (!paths || !*paths)
	{
        //printf("paths is NULL\n");
        return ;
    }
	//printf("paths address: %p\n", paths);
	while ((*paths)[i])
	{
		if ((*paths)[i])
		{
			//printf("Freeing paths[%d] at %p: %s\n", i, paths[i], paths[i]);
			free((*paths)[i]);
		}
		i++;
	}
	free(*paths);
	*paths = NULL;
}

char	*ft_get_path_from_env(t_env *env)
{
	while (env)
	{
		if (ft_strncmp(env->name, "PATH", 5) == 0)
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
			return (ft_free_array(&dirs), cmd_path);
		free(cmd_path);
		i++;
	}
	ft_free_array(&dirs);
	return (NULL);
}
