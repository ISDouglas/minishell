/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fill_utiles.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 17:58:55 by layang            #+#    #+#             */
/*   Updated: 2025/05/11 14:40:49 by layang           ###   ########.fr       */
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
	t_token	*cur;
	int		count;

	ft_free_array(&mini->cmd_array);
	cur = *tokens;
	count = 0;
	while (cur)
	{
		if (cur->cmd)
			count++;
		cur = cur->next;
	}
	if (count == 0)
		safe_exit_child(mini, "minishell: no command", 0);
	mini->cmd_array = malloc(sizeof(char *) * (count + 1));
	if (!mini->cmd_array)
	{
		(void)ft_link_status("malloc cmd_line failed", 1);
		return (-1);
	}
	ft_memset(mini->cmd_array, 0, sizeof(char *) * (count + 1));
	return (0);
}

void	endby_eof_delimiter(int d, char	*line, char	*de, t_mini	*mi)
{
	if (d)
	{
		free(line);
		get_next_line(-1);
	}
	else
	{
		dup2(mi->stdout_fd, 1);
		printf("minshell warning: heredoc delimited by EOF (wanted '%s')\n",
			de);
		dup2(mi->log_fd, 1);
	}
}
