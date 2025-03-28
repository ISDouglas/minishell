/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:30:21 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/27 18:00:25 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_env *env)
{
	t_env	*current;

	if (!env)
		return (FAILURE);
	current = env;
	while (current)
	{
		write(1, current->name, ft_strlen(current->name));
		write(1, "=", 1);
		write(1, current->value, ft_strlen(current->value));
		write(1, "\n", 1);
		current = current->next;
	}
	return (SUCCESS);
}

/*int	ft_unset(t_token *tokens, t_env *env)
{
	t_env	*current;
	t_env	*prev;

	if (!tokens->next)
	{
		perror("unset: not enough arguments");
		return (SUCCESS);
	}
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
}*/

int	ft_isbuilt_in(char *cmd, t_token *tokens, t_env *env)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ft_echo(tokens));
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (ft_cd(tokens, env));
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (ft_export(tokens, &env));
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (ft_unset(tokens, &env));
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (ft_env(env));
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		exit(0);
	return (FAILURE);
}
