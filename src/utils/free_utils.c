/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 19:08:55 by layang            #+#    #+#             */
/*   Updated: 2025/05/14 12:22:47 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_array(char	***paths)
{
	int	i;

	i = 0;
	if (!paths || !*paths || !**paths)
		return ;
	while ((*paths)[i])
	{
		if ((*paths)[i])
			free((*paths)[i]);
		i++;
	}
	free(*paths);
	*paths = NULL;
}

void	ft_free_tokens(t_token	**tokens)
{
	t_token	*tmp;

	if (!tokens || !*tokens)
		return ;
	while (*tokens)
	{
		tmp = *tokens;
		if (tmp->value)
		{
			free(tmp->value);
			tmp->value = NULL;
		}
		if (tmp->cmd)
		{
			free(tmp->cmd);
			tmp->cmd = NULL;
		}
		*tokens = (*tokens)->next;
		free(tmp);
	}
	*tokens = NULL;
}

void	ft_close_cmd_fd(t_token	*tokens)
{
	int		old_in;
	int		old_out;
	t_token	*cur;

	cur = tokens;
	old_in = -2;
	old_out = -3;
	while (cur)
	{
		if (cur->infile != 0 && cur->infile != -1 && cur->infile != old_in)
		{
			old_in = cur->infile;
			close(cur->infile);
		}
		if (cur->outfile != 1 && cur->outfile != -1
			&& cur->outfile != old_out)
		{
			old_out = cur->outfile;
			close(cur->outfile);
		}
		cur = cur->next;
	}
}

static void	ft_free_mini_0(t_mini	*all)
{
	int	i;

	if (all->exe_tab)
	{
		i = 0;
		while (i < all->tab_size)
		{
			if (all->exe_tab[i] != NULL)
			{
				ft_close_cmd_fd(all->exe_tab[i]);
				ft_free_tokens(&all->exe_tab[i]);
			}
			i++;
		}
		free(all->exe_tab);
		all->exe_tab = NULL;
	}
	if (all->cmd_array)
		ft_free_array(&all->cmd_array);
	if (all->lexer)
		ft_free_tokens(&all->lexer);
}

void	ft_free_mini(t_mini *all, int sign)
{
	if (sign == 1)
	{
		if (all->env)
			ft_free_env(&all->env);
		if (all->array_env)
			ft_free_array(&all->array_env);
		close(all->stdout_fd);
		close(all->log_fd);
		clear_history();
	}
	ft_free_mini_0(all);
	free(all->input);
	all->input = NULL;
	if (all->cpid)
	{
		free(all->cpid);
		all->cpid = NULL;
	}
}
