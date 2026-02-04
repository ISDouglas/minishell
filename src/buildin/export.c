/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:26:37 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/11 13:55:58 by nimorel          ###   ########.fr       */
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
				current->env_flag = 1;
			}
			free(name);
			return (SUCCESS);
		}
		current = current->next;
	}
	if (!ft_strcmp("OLDPWD", name) || !ft_strcmp("PWD", name))
	{
		free(value);
		free(name);
	}
	return (FAILURE);
}

int	ft_add_var(t_env **env, char *name, char *value, int d)
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
	new_node->env_flag = d;
	if (*env)
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new_node;
	}
	else
		*env = new_node;
	return (free(name), free(value), SUCCESS);
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

int	ft_export(t_token *tokens, t_env **env, t_mini *mini)
{
	char	*name;
	int		ex;

	if (!tokens->next || (mini->cmd_array[0] && !mini->cmd_array[1]))
		return (ft_export_no_arg(*env), 0);
	ex = 0;
	tokens = tokens->next;
	while (tokens)
	{
		name = ft_extract_name(tokens->value);
		if (name && ft_is_valid_name(name))
			ft_handle_valid_export(env, tokens->value, name);
		else
		{
			if (name)
			{
				printf("export: %s: not a valid identifier\n", name);
				ex = 1;
				free(name);
			}
		}
		tokens = tokens->next;
	}
	return (ex);
}
