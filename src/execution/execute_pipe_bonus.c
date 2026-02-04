/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:33:31 by layang            #+#    #+#             */
/*   Updated: 2025/05/08 07:43:06 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	ft_execute_last(t_mini *mini, int i, t_env	**env)
{
	t_token	*cur;

	ft_fill_cmd(&mini->exe_tab[i], mini);
	dup2(mini->log_fd, 1);
	printf("\n---fill last cmd, line %d: \n", i);
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
		mini->pre = -1;
		ft_exe_cmd(mini, i, env);
	}
}

void	ft_execute_child(t_mini *mini, int i, int pipe_fd[2], t_env	**env)
{
	t_token	*cur;

	ft_fill_cmd(&mini->exe_tab[i], mini);
	printf("\n---fill child cmd: \n");
	ft_print_token(mini->exe_tab[i]);
	dup2(mini->stdout_fd, STDOUT_FILENO);
	if (mini->cmd_array)
	{
		cur = mini->exe_tab[i];
		while (cur->next)
			cur = cur->next;
		if (cur->infile == STDIN_FILENO
			&& (i - 2 >= 0 && next_type(mini, i - 2) == PIPE))
			dup2(mini->pre, 0);
		else
			dup2(cur->infile, 0);
		if (cur->outfile != STDOUT_FILENO)
			dup2(cur->outfile, 1);
		else
			dup2(pipe_fd[1], 1);
		close(pipe_fd[0]);
		ft_exe_cmd(mini, i, env);
	}
}

void	ft_signal_in_child(t_mini *mini, int status)
{
	dup2(mini->stdout_fd, 1);
	if (WTERMSIG(status) == SIGQUIT)
		write(1, "Quit (core dumped)\n", 20);
	else if (WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	dup2(mini->log_fd, 1);
}

/* void	ft_wait_bonus(t_mini	*mini, int i, int	*len)
{
	if (i == mini->tab_size - 1 || next_type(mini, i) != PIPE)
	{
		ft_wait_children(mini, *len + 1);
		*len = 0;
	}
	else
		(*len)++;
} */

void	ft_wait_bonus(t_mini	*mini, int i, int	*len)
{
	if (len == 0 || next_type(mini, i) == PIPE)
		return ;
	else
	{
		ft_wait_children(mini, *len);
		*len = 0;
	}
}

void	ft_wait_children(t_mini	*mini, int len)
{
	int	i;
	int	status;
	int	last_exit;

	i = 0;
	last_exit = 0;
	while (i < len)
	{
		waitpid(mini->cpid[i], &status, 0);
		if (i++ == len - 1)
		{
			if (WIFEXITED(status))
				last_exit = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				ft_signal_in_child(mini, status);
				last_exit = 128 + WTERMSIG(status);
			}
		}
	}
	ft_memset(mini->cpid, 0, sizeof(int) * mini->tab_size);
	printf("\nlast-g_status **: %d\n", last_exit);
	(void)ft_link_status(NULL, last_exit);
}

/* void	ft_wait_children(t_mini	*mini)
{
	int	i;
	int	status;
	int	last_exit;

	i = 0;
	last_exit = 0;
	while (i < mini->tab_size)
	{
		waitpid(mini->cpid[i], &status, 0);
		if (WIFSIGNALED(status))
		{
			ft_signal_in_child(mini, status);
			last_exit = 128 + WTERMSIG(status);
			break ;
		}
		if (i++ == mini->tab_size - 1)
			if (WIFEXITED(status))
				last_exit = WEXITSTATUS(status);
	}
	printf("\nlast-g_status **: %d\n", last_exit);
	(void)ft_link_status(NULL, last_exit);
} */
