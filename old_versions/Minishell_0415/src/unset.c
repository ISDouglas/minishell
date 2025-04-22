/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:59:12 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/09 20:53:32 by layang           ###   ########.fr       */
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
