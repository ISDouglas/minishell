/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_jump.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:39:17 by layang            #+#    #+#             */
/*   Updated: 2025/05/08 03:03:35 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	ft_is_operator(t_token	*cur)
{
	if (cur->type == PIPE || cur->type == AND || cur->type == OR)
		return (1);
	else
		return (0);
}

t_token_type	next_type(t_mini	*mini, int i)
{
	if (i + 1 < mini->tab_size)
		return (mini->exe_tab[i + 1]->type);
	else
		return (-1);
}

// if pipe, back one, if not back to pass_or_jump
static void	jump_over_par(int old, t_mini	*mini, int	*i)
{
	while (*i < mini->tab_size && mini->exe_tab[*i]->par_n > old)
		(*i)++;
	if (*i >= mini->tab_size)
		return ;
	if (mini->exe_tab[*i]->type == PIPE)
	{
		(*i)--;
		return ;
	}
	else
	{
		(*i)--;
		pass_or_jump(mini, i);
	}
}

static void	jump_with_level(int code, t_mini	*mini, int	*i)
{
	int	level;

	if (*i < mini->tab_size - 2 && mini->exe_tab[*i + 2])
	{
		level = mini->exe_tab[*i + 1]->par_n;
		(*i) = (*i) + 2;
		if (mini->exe_tab[*i]->par_n == level)
		{
			if (next_type(mini, *i) != PIPE)
				pass_or_jump(mini, i);
			else
				jump_with_level(code, mini, i);
		}
		else
			jump_over_par(level, mini, i);
	}
	else
	{
		(*i)++;
		return ;
	}
}

void	pass_or_jump(t_mini	*mini, int	*i)
{
	int	code;

	if (*i == mini->tab_size - 1)
	{
		(*i)++;
		return ;
	}
	code = ft_link_status(NULL, -1);
	printf("code of cmd[%d]: %d\n", *i, code);
	if (next_type(mini, *i) == PIPE || (code == 0 && next_type(mini, *i) == AND)
		|| (code != 0 && next_type(mini, *i) == OR))
		(*i) = (*i) + 2;
	else
		jump_with_level(code, mini, i);
}
