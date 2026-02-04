/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:30:21 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/11 13:41:33 by nimorel          ###   ########.fr       */
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
	while (current && current->env_flag == 1)
	{
		write(1, current->name, ft_strlen(current->name));
		write(1, "=", 1);
		if (current->value)
			write(1, current->value, ft_strlen(current->value));
		write(1, "\n", 1);
		current = current->next;
	}
	return (SUCCESS);
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

int	is_buildin_single(t_mini	*mini)
{
	char	*cmd;

	cmd = mini->exe_tab[0]->value;
	if (mini->tab_size == 1 && (!ft_strcmp(cmd, "cd")
			|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
			|| !ft_strcmp(cmd, "exit")))
		return (1);
	else
		return (0);
}

void	ft_cd_export_unset(t_mini *mini, int i)
{
	char	*cmd;
	int		ex;
	t_token	*arg;

	cmd = mini->exe_tab[i]->value;
	arg = mini->exe_tab[i]->next;
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0)
	{
		ft_execute_simple_cmd(mini, i, 1);
		ex = ft_is_built_in(cmd, mini->exe_tab[i], mini);
		(void)ft_link_status(NULL, ex);
	}
	if (ft_strcmp(cmd, "exit") == 0)
	{
		ex = ft_exit(mini, i);
		(void)ft_link_status(NULL, ex);
		if (!(arg && ft_is_numeric(arg->value) && arg->next))
		{
			ft_free_mini(mini, 1);
			exit(ex);
		}
	}
}
