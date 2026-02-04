/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 14:42:09 by layang            #+#    #+#             */
/*   Updated: 2025/05/10 18:15:29 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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

int	ft_is_built_in(char *cmd, t_token *tokens, t_mini *mini, t_env	**env)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(mini->cmd_array, mini));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(tokens, *env));
	else if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(tokens, env, mini));
	else if (ft_strcmp(cmd, "unset") == 0)
		return (ft_unset(tokens, env));
	else
		return (ft_env(*env));
}

/* is_buildin_single(t_mini *mini, int i)
{
	char	*cmd;
	int		par;

	cmd = mini->exe_tab[i]->value;
	par = mini->exe_tab[i]->par_n;
	if (mini->tab_size == 1 && par == 0 && (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "exit")))
			return (1);
	else
		return (0);
} */

int	is_buildin_single(t_mini *mini, int i)
{
	char	*cmd;
	int		par;

	cmd = mini->exe_tab[i]->value;
	par = mini->exe_tab[i]->par_n;
	if (par == 0 && (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit")))
		return (1);
	else if (!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"))
		return (1);
	else
		return (0);
}

//	printf("layer is : %d\n", layer);
void	ft_cd_export_unset(t_mini *mini, int i)
{
	char	*cmd;
	int		ex;
	int		layer;
	t_env	*env;

	cmd = ft_strdup(mini->exe_tab[i]->value);
	layer = count_layer(mini, i);
	if (layer == 0)
		buildin_main_shell(cmd, mini, i);
	else
	{
		env = mini->sub_env[layer - 1];
		if (ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "export") == 0)
		{
			ft_execute_simple_cmd(mini, i, 1, &env);
			ex = ft_is_built_in(cmd, mini->exe_tab[i], mini, &env);
			(void)ft_link_status(NULL, ex);
			free(cmd);
		}
	}
}
