/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:59:12 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/08 08:13:58 by layang           ###   ########.fr       */
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

int	ft_is_valid_name(const char	*name)
{
	int	i;

	i = 0;
	if (!name || !name[0])
		return (0);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	while (name[i])
	{
		if (name[i] == '=')
			break ;
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	if (ft_strchr(name, ' ') || ft_strchr(name, '\t'))
		return (0);
	return (1);
}

void	ft_handle_valid_export(t_env	**env, char	*str, char	*name)
{
	char	*value;

	value = ft_strchr(str, '=');
	if (value)
		value = ft_strdup(value + 1);
	if (ft_update_var(*env, name, value) != SUCCESS)
		ft_add_var(env, name, value);
}
