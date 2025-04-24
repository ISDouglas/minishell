/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:30:21 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/09 18:05:01 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_mini	*mini, int i)
{
	long long		nb;

	ft_putstr_fd("exit\n", 2);
	if (mini->exe_tab[i]->next)
	{
		if (!ft_is_numeric(mini->exe_tab[i]->next->value))
			return (write(2, "exit: numeric argument required\n", 32), 2);
		if (!ft_atoll_code(mini->exe_tab[i]->next->value, &nb))
			return (write(2, "exit: numeric argument required\n", 32), 2);
		else
		{
			if (mini->exe_tab[i]->next->next)
				return (ft_putstr_fd("exit: too many arguments\n", 2), 1);
			return ((unsigned int)nb);
		}
	}
	else
		return (ft_link_status(NULL, -1));
}

int	ft_env(t_env *env)
{
	t_env	*current;

	if (!env)
		return (FAILURE);
	current = env;
	while (current)
	{
		if (ft_strcmp(current->value, "") != 0)
		{
			write(1, current->name, ft_strlen(current->name));
			write(1, "=", 1);
			write(1, current->value, ft_strlen(current->value));
			write(1, "\n", 1);
		}
		current = current->next;
	}
	return (SUCCESS);
}

int	ft_cmd_type(char *cmd)
{
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0)
		return (BUILT_IN_CMD);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (EXIT_CMD);
	else
		return (NOT_BUILT_IN_CMD);
}

int	ft_is_built_in(char *cmd, t_token *tokens, t_mini *mini)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(mini->cmd_array, mini));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(tokens, mini->env));
	else if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(tokens, &mini->env, mini));
	else if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(tokens, &mini->env));
	else
		return (ft_env(mini->env));
}

void	ft_cd_export_unset(t_mini *mini, int i)
{
	char	*cmd;

	cmd = strdup(mini->lexer->value);
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0)
	{
		ft_execute_simple_cmd(mini, i, 1);
		(void)ft_is_built_in(cmd, mini->lexer, mini);
		free(cmd);
	}
	else if (ft_strcmp(cmd, "exit") == 0)
	{
		free(cmd);
		if (!(mini->exe_tab[i]->next
				&& ft_is_numeric(mini->exe_tab[i]->next->value)
				&& mini->exe_tab[i]->next->next))
		{
			ft_free_mini(mini, 1);
			exit(ft_link_status(NULL, -1));
		}
	}
	else
		free(cmd);
}
