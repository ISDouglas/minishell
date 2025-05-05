/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fill_utiles.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:58:55 by layang            #+#    #+#             */
/*   Updated: 2025/04/26 18:01:34 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pass_in_out(t_token	**token)
{
	if ((*token)->next)
	{
		(*token)->next->infile = (*token)->infile;
		(*token)->next->outfile = (*token)->outfile;
	}
}

int	ft_malloc_array(t_token **tokens, t_mini *mini)
{
	int		i;
	t_token	*cur;

	ft_free_array(&mini->cmd_array);
	cur = *tokens;
	i = 0;
	while (cur)
	{
		cur = cur->next;
		i++;
	}
	mini->cmd_array = malloc(sizeof(char *) * (i + 1));
	if (!mini->cmd_array)
	{
		(void)ft_link_status("malloc cmd_line failed", 1);
		return (-1);
	}
	ft_memset(mini->cmd_array, 0, sizeof(char *) * (i + 1));
	return (0);
}
