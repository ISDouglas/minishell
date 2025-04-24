/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:33:31 by layang            #+#    #+#             */
/*   Updated: 2025/04/15 17:33:36 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_check_last_cmd(t_mini *mini, int i)
{
	if (!mini->exe_tab[i] || !mini->exe_tab[i]->value)
	{
		printf("Minishell is not suppose to accept empty last cmd after | .");
		return (1);
	}
	return (0);
}

void	ft_execute_last(t_mini *mini, int i)
{
	t_token	*cur;

	if (ft_check_last_cmd(mini, i))
		return ;
	ft_fill_cmd(&mini->exe_tab[i], mini);
	dup2(mini->log_fd, 1);
	printf("\n---fill last cmd: \n");
	ft_print_token(mini->exe_tab[i]);
	dup2(mini->stdout_fd, STDOUT_FILENO);
	if (mini->cmd_array)
	{
		close(mini->log_fd);
		cur = mini->exe_tab[i];
		while (cur->next)
			cur = cur->next;
		if (cur->infile == STDIN_FILENO)
			dup2(mini->pre, 0);
		else
			dup2(cur->infile, 0);
		if (cur->outfile != STDOUT_FILENO)
			dup2(cur->outfile, 1);
		close(mini->pre);
		ft_exe_cmd(mini, i);
	}
}

void	ft_execute_child(t_mini *mini, int i, int pipe_fd[2])
{
	t_token	*cur;

	ft_fill_cmd(&mini->exe_tab[i], mini);
	printf("\n---fill child cmd: \n");                  // ** test code
	ft_print_token(mini->exe_tab[i]);
	dup2(mini->stdout_fd, STDOUT_FILENO);
	if (mini->cmd_array)
	{
		cur = mini->exe_tab[i];
		while (cur->next)
			cur = cur->next;
		if (cur->infile == STDIN_FILENO && i != 0)
			dup2(mini->pre, 0);
		else
			dup2(cur->infile, 0);
		if (cur->outfile != STDOUT_FILENO)
			dup2(cur->outfile, 1);
		else
			dup2(pipe_fd[1], 1);
		close(pipe_fd[0]);
		ft_exe_cmd(mini, i);
	}
}
