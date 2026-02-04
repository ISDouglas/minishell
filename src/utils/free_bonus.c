/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:36:49 by layang            #+#    #+#             */
/*   Updated: 2025/05/08 08:17:46 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	ft_free_sub_env(t_env	***sub_env, int layer)
{
	int	size;
	int	i;

	size = layer - 1;
	i = 0;
	while (i < size)
	{
		if ((*sub_env)[i])
			ft_free_env(&(*sub_env)[i]);
		i++;
	}
	free(*sub_env);
	*sub_env = NULL;
}

int	count_layer(t_mini	*mini, int i)
{
	int	layer;

	if ((i - 2 >= 0 && next_type(mini, i - 2) == PIPE)
		&& mini->exe_tab[i - 2]->par_n > mini->exe_tab[i]->par_n)
		layer = mini->exe_tab[i - 2]->par_n;
	else
		layer = mini->exe_tab[i]->par_n;
	return (layer);
}

void	buildin_main_shell(char	*cmd, t_mini	*mini, int i)
{
	int		ex;
	t_token	*arg;

	arg = mini->exe_tab[i]->next;
	if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0)
	{
		ft_execute_simple_cmd(mini, i, 1, &mini->env);
		ex = ft_is_built_in(cmd, mini->exe_tab[i], mini, &mini->env);
		(void)ft_link_status(NULL, ex);
		free(cmd);
	}
	else if (ft_strcmp(cmd, "exit") == 0)
	{
		free(cmd);
		ex = ft_exit(mini, i);
		(void)ft_link_status(NULL, ex);
		if (!(arg && ft_is_numeric(arg->value) && arg->next))
		{
			ft_free_mini(mini, 1);
			exit(ex);
		}
	}
	else
		free(cmd);
}
