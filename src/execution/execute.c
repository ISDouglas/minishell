/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:43 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/07 09:30:55 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_simple_cmd(t_mini	*mini, int i, int sign)
{
	t_token	*cur;

	printf("\n**start fill simple cmd_array\n");
	ft_fill_cmd(&mini->exe_tab[i], mini);
	dup2(mini->stdout_fd, STDOUT_FILENO);
	if (mini->cmd_array)
	{
		cur = mini->exe_tab[i];
		while (cur->next)
			cur = cur->next;
		dup2(cur->infile, 0);
		dup2(cur->outfile, 1);
		if (sign == 0)
			close(mini->log_fd);
		if (sign == 0)
			ft_exe_cmd(mini, i);
	}
}

void	ft_execute_parent(t_mini	*mini, int i, int pipe[2], int pid)
{
	if (mini->tab_size != 1 && i != mini->tab_size - 1)
	{
		printf("          pipe[1] before close: %d\n", pipe[1]);
		close(pipe[1]);
	}
	mini->cpid[i] = pid;
	if (mini->pre != -1)
	{
		printf("          pre before close: %d\n", mini->pre);
		close(mini->pre);
	}
	if (mini->tab_size != 1 && i != mini->tab_size - 1)
	{
		printf("pre (before assign): %d \n", mini->pre);
		mini->pre = pipe[0];
	}
	if (mini->tab_size > 1 && i != mini->tab_size - 1)
		printf("pre(after assign) = %d, pipe[0] = %d \n", mini->pre, pipe[0]);
}

static void	ft_handle_child(t_mini	*mini, int i, int *pipe_fd)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, ft_handle_sigint_2);
	if (mini->tab_size == 1)
		ft_execute_simple_cmd(mini, i, 0);
	else if (i != mini->tab_size - 1)
		ft_execute_child(mini, i, pipe_fd);
	else
		ft_execute_last(mini, i);
}

void	ft_execute_unit(t_mini	*mini, int i)
{
	int	pipe_fd[2];
	int	pid;

	if (mini->tab_size != 1 && i != mini->tab_size - 1)
	{
		if (pipe(pipe_fd) == -1)
		{
			(void)ft_link_status("pipe", 1);
			return ;
		}
		printf("\n   ##(pipe0: %d, pipe1: %d)##\n", pipe_fd[0], pipe_fd[1]);
	}
	pid = fork();
	if (pid < 0)
	{
		(void)ft_link_status("fork", 1);
		return ;
	}
	if (pid == 0)
		ft_handle_child(mini, i, pipe_fd);
	ft_execute_parent(mini, i, pipe_fd, pid);
}

void	ft_execute(t_mini	*mini)
{
	int	i;

	i = 0;
	if (is_buildin_single(mini))
	{
		ft_cd_export_unset(mini, 0);
		return ;
	}
	mini->cpid = malloc(sizeof(int) * mini->tab_size);
	if (!mini->cpid)
	{
		(void)ft_link_status("malloc cpid failed", 1);
		return ;
	}
	while (i < mini->tab_size)
	{
		ft_execute_unit(mini, i);
		dup2(mini->log_fd, 1);
		i++;
	}
	ft_wait_children(mini);
}
