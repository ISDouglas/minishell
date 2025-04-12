/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:59:12 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/12 14:41:21 by nimorel          ###   ########.fr       */
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
int ft_unset(t_token *tokens, t_env **env)
{
	t_env *current;
	t_env *prev;
	t_token *arg;
	t_env *tmp;

	if (!tokens->next)
	{
		perror("unset: not enough arguments");
		return (SUCCESS);
	}
	arg = tokens->next;
	while (arg)
	{
		current = *env;
		prev = NULL;
		while (current)
		{
			if (ft_strcmp(current->name, arg->value) == 0)
			{
				tmp = current->next;
				ft_delete_var(env, prev, current);
				current = tmp;
				continue;
			}
			prev = current;
			current = current->next;
		}
		arg = arg->next;
	}
	return (SUCCESS);
} 
/*int	ft_unset(t_token *tokens, t_env **env)
{
	t_env	*current;
	t_env	*prev;

	if (!tokens->next)
	{
		perror("unset: not enough arguments");
		return (SUCCESS);
	}
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, tokens->next->value) == 0)
		{
			ft_delete_var(env, prev, current);
			return (SUCCESS);
		}
		prev = current;
		current = current->next;
	}
	return (FAILURE);
}*/
