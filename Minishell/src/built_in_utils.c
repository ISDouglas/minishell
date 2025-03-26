/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 12:31:23 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/26 14:19:46 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_get_env(t_token *tokens, t_env *env)
{
	t_env	*current;

	
	current = env;
	if (tokens->next == NULL)
	{
		while (current)
		{
			if (current->value)
			{
				write(1, current->name, ft_strlen(current->name));
				write(1, "=", 1);
				write(1, current->value, ft_strlen(current->value));
				write(1, "\n", 1);
			}
			current = current->next;
		}
	}
	else if (tokens->next->type == ENV_VAR)
		{
		while (current)
		{
			if (ft_strcmp(current->name, &tokens->next->value[1]) == 0)
			{
				write(1, current->value, ft_strlen(current->value));
			}
			current = current->next;
		}
	}
	return (SUCCESS);
}

int	ft_unset(t_token *tokens, t_env *env)
{
	t_env	*current;
	t_env	*prev;

	current = env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, tokens->next->value) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				env = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return (SUCCESS);
		}
		prev = current;
		current = current->next;
	}
	return (FAILURE);
}
char *ft_extract_name(char *str)
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

int	ft_export(t_token *tokens, t_env **env)
{
	t_env	*current;
	t_env	*new_node;
	char	*name;
	char	*value;
	
	printf("ptr env %p\n", env);
	if (!tokens->next)
	{
		ft_get_env(tokens, *env);
		return (SUCCESS);
	}
	tokens = tokens->next;
	name = ft_extract_name(tokens->value);
	value = ft_strchr(tokens->value, '=');
	if (value)
		value++;
	else
		value = "";
	current = *env;
	while (current)
	{
		
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			free(name);
			return (SUCCESS);
		}
		current = current->next;
	}
	new_node = ft_create_env_node(name, value);
	free(name);
	if (!new_node)
	{
		perror("Failed to create new env node");
		return (FAILURE);
	}
	new_node->next = *env;
	*env = new_node;
	printf("ptr env %p\n", env);
	printf("fisrt env %s\n", env[1]->name);
	return (SUCCESS);
}
