/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:26:37 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/12 14:21:04 by nimorel          ###   ########.fr       */
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
	return (FAILURE);
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
		free(value);
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
	free(name);
	free(value);
	return (SUCCESS);
}

static int	ft_export_no_arg(t_env *env)
{
	t_env	*current;

	if (!env)
		return (FAILURE);
	current = env;
	while (current)
	{
		write(1, "declare -x ", 11);
		write(1, current->name, ft_strlen(current->name));
		if (current->value != NULL)
		{
			write(1, "=", 1);
			write(1, "\"", 1);
			write(1, current->value, ft_strlen(current->value));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		current = current->next;
	}
	return (SUCCESS);
}

int	ft_export(t_token *tokens, t_env **env)
{
	char	*name;
	char	*value;

	if (!tokens->next)
	{
		ft_export_no_arg(*env);
		return (SUCCESS);
	}
	tokens = tokens->next;
	while (tokens)
	{
		name = ft_extract_name(tokens->value);
		if (!name)
			tokens = tokens->next;
		value = ft_strchr(tokens->value, '=');
		if (value)
			value = ft_strdup(value + 1);
		if (ft_update_var(*env, name, value) != SUCCESS)
			ft_add_var(env, name, value);
		tokens = tokens->next;
	}
	return (SUCCESS);
}
