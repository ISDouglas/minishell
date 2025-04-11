/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:57:41 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/09 19:40:19 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0' && s1[i] == s2[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


void	ft_close_cmd_fd(t_token	*tokens)
{
	int	old_in;
	int	old_out;
	t_token	*cur;
	
	cur = tokens;
	old_in = -2;
	old_out = -3;
	while (cur)
	{
		if (cur->infile != 0 && cur->infile != old_in)
		{
			old_in = cur->infile;
			close(cur->infile);
		}
		if (cur->outfile != 1 && cur->outfile != old_out)
		{
			old_out = cur->outfile;
			close(cur->outfile);
		}
		cur = cur->next;
	}
}

static void	ft_free_mini_0(t_mini *all)
{
	int	i;
	
	if (all->exe_tab)
	{
		i = 0;
		while (i < all->tab_size)
		{
			//printf("\n**Free tab now, i: %d, address: %p\n", i, all->exe_tab[i]);
			if (all->exe_tab[i] != NULL)
			{
				ft_close_cmd_fd(all->exe_tab[i]);
				ft_free_tokens(all->exe_tab[i]);
			}
			i++;
		}
		free(all->exe_tab);
		all->exe_tab = NULL;
	}
	if (all->cmd_array)
		ft_free_array(&all->cmd_array);
	if (all->lexer)
	{
		ft_free_tokens(all->lexer);
		all->lexer = NULL;
	}
}

void	ft_free_mini(t_mini *all, int sign)
{
	if (sign == 0 || sign == 1)
	{
		ft_free_mini_0(all);
		free(all->input);
		all->input = NULL;
	}
	if (sign == 1)
	{
		if (all->env)
		{
			//printf("\n*Free env now, address: %p\n", all->env);
			ft_free_env(&all->env);
		}			
		if (all->array_env)
		{
			//printf("\n*Free array env now, address: %p\n\n", all->env);
			ft_free_array(&all->array_env);
		}		
	}
}
