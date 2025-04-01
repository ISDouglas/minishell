/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:26:37 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/29 08:08:29 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_extract_name(char *str)
{
	char	*name;
	int		i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	name = malloc(i + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, str, i + 1);
	return (name);
}

int	ft_update_var(t_env *env, char *name, char *value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (value)
			{
				free(current->value);
				current->value = value;
			}
			free(name);
			return (SUCCESS);
		}
		current = current->next;
	}
	return (printf("update env: update %s failed\n", name), FAILURE);
}

static int	ft_add_var(t_env **env, char *name, char *value)
{
	t_env	*new_node;
	t_env	*current;

	if (!value)
		value = ft_strdup("");
	new_node = ft_create_env_node(name, value);
	if (!new_node)
	{
		perror("Failed to create new env node");
		free(name);
		return (FAILURE);
	}
	if (*env)
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	else
		*env = new_node;
	return (SUCCESS);
}

int	ft_export(t_token *tokens, t_env **env)
{
	char	*name;
	char	*value;

	if (!tokens->next)
	{
		ft_env(*env);
		return (SUCCESS);
	}
	tokens = tokens->next;
	name = ft_extract_name(tokens->value);
	if (!name)
		return (FAILURE);
	value = ft_strchr(tokens->value, '=');
	if (value)
		value = ft_strdup(value + 1);
	if (ft_update_var(*env, name, value) == SUCCESS)
		return (SUCCESS);
	return (ft_add_var(env, name, value));
}
