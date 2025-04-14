/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:59:12 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/27 18:02:05 by nimorel          ###   ########.fr       */
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

int	ft_unset(t_token *tokens, t_env **env)
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
}