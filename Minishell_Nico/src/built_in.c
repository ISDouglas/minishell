/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:30:21 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/29 14:58:19 by nimorel          ###   ########.fr       */
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

int	ft_isbuilt_in(char *cmd, t_token *tokens, t_mini *mini)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ft_echo(tokens, mini->env));
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (ft_cd(tokens, mini->env));
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (ft_export(tokens, &mini->env));
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (ft_unset(tokens, &mini->env));
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (ft_env(mini->env));
	else if (ft_strncmp(cmd, "exit", 4) == 0)
	{
		status = 0;
		return (EXIT_CMD);
	}
	return (NOT_BUILT_IN_CMD);
}
