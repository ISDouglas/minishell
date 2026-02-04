/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:59:12 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/10 18:53:28 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_delete_var(t_env **env, t_env *prev, t_env *current)
{
	if (prev)
		prev->next = current->next;
	else
		*env = current->next;
	free(current->name);
	free(current->value);
	free(current);
}

static void	ft_unset_process(t_env **env, t_token *arg)
{
	t_env	*current;
	t_env	*prev;
	t_env	*tmp;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, arg->value) == 0)
		{
			tmp = current->next;
			ft_delete_var(env, prev, current);
			current = tmp;
			continue ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(t_token *tokens, t_env **env)
{
	t_token	*arg;

	if (!tokens->next)
	{
		perror("unset: not enough arguments");
		return (SUCCESS);
	}
	arg = tokens->next;
	while (arg)
	{
		ft_unset_process(env, arg);
		arg = arg->next;
	}
	return (SUCCESS);
}

static int	name_exit_env(char	*name, t_env	**env)
{
	t_env	*current;

	current = *env;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (1);
		current = current->next;
	}
	return (0);
}

void	ft_handle_valid_export(t_env	**env, char	*str, char	*name)
{
	char	*value;
	int		d;

	value = ft_strchr(str, '=');
	if (value)
	{
		d = 1;
		value = ft_strdup(value + 1);
	}
	else
	{
		if (name_exit_env(name, env))
		{
			free(name);
			return ;
		}
		d = 0;
		value = ft_strdup("");
	}
	if (ft_update_var(*env, name, value) != SUCCESS)
		ft_add_var(env, name, value, d);
}
