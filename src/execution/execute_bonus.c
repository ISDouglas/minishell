/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:43 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/08 08:18:15 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	ft_execute_simple_cmd(t_mini	*mini, int i, int sign, t_env	**env)
{
	t_token	*cur;

	printf("\n**start fill simple cmd_array\n");
	ft_tabn_reread(mini, mini->exe_tab[i]->start, i);
	ft_fill_cmd(&mini->exe_tab[i], mini);
	dup2(mini->stdout_fd, STDOUT_FILENO);
	if (mini->cmd_array)
	{
		cur = mini->exe_tab[i];
		while (cur->next)
			cur = cur->next;
		if (cur->infile == STDIN_FILENO)
			dup2(mini->pre, 0);
		else
			dup2(cur->infile, 0);
		if (cur->outfile != STDOUT_FILENO)
			dup2(cur->outfile, 1);
		if (sign == 0)
			close(mini->log_fd);
		if (sign == 0)
			ft_exe_cmd(mini, i, env);
	}
}

void	ft_execute_parent(t_mini	*mini, int i, int pipe[2])
{
	if (mini->pre != -1)
	{
		printf("----------pre before close: %d\n", mini->pre);
		close(mini->pre);
		mini->pre = -1;
	}
	if (next_type(mini, i) == PIPE)
	{
		printf("----------pipe[1] before close: %d\n", pipe[1]);
		close(pipe[1]);
		printf("----------pre (before assign): %d \n", mini->pre);
		mini->pre = pipe[0];
		printf("pre(after assign) = %d, pipe[0] = %d \n", mini->pre, pipe[0]);
	}
}

static void	ft_handle_child(t_mini	*mini, int i, int *pipe_fd, int layer)
{
	t_env	*env;

	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, ft_handle_sigint_2);
	printf("\n--CMD %s layer: %d\n", mini->exe_tab[i]->value, layer);
	if (layer == 0)
		env = mini->env;
	else
		env = mini->sub_env[layer - 1];
	if ((i - 2 >= 0 && next_type(mini, i - 2) == PIPE)
		|| next_type(mini, i) == PIPE)
	{
		if (next_type(mini, i) == PIPE)
			ft_execute_child(mini, i, pipe_fd, &env);
		else
			ft_execute_last(mini, i, &env);
	}
	else
		ft_execute_simple_cmd(mini, i, 0, &env);
}

void	ft_execute_unit(t_mini	*mini, int i, int *len, int layer)
{
	int	pipe_fd[2];
	int	pid;

	if (next_type(mini, i) == PIPE)
	{
		reread_pipe_chain(mini, i);
		if (pipe(pipe_fd) == -1)
			return ((void)ft_link_status("pipe", 1));
		printf("\n   ##(pipe0: %d, pipe1: %d)##\n", pipe_fd[0], pipe_fd[1]);
	}
	pid = fork();
	if (pid < 0)
		return ((void)ft_link_status("fork", 1));
	if (pid == 0)
		ft_handle_child(mini, i, pipe_fd, layer);
	mini->cpid[*len] = pid;
	(*len)++;
	ft_execute_parent(mini, i, pipe_fd);
}

void	ft_execute(t_mini	*mini)
{
	int	i;
	int	len;
	int	layer;

	i = 0;
	len = 0;
	while (i < mini->tab_size)
	{
		layer = dup_sub_shell(mini, i);
		if (layer < 0)
			return ((void)ft_link_status("dup sub shell env", 1));
		if (!((i - 2 >= 0 && next_type(mini, i - 2) == PIPE)
				|| (next_type(mini, i) == PIPE)) && is_buildin_single(mini, i))
			ft_cd_export_unset(mini, i);
		else
		{
			ft_execute_unit(mini, i, &len, layer);
			ft_wait_bonus(mini, i, &len);
		}
		dup2(mini->log_fd, 1);
		pass_or_jump(mini, &i);
	}
}

//if (mini->tab_size == 1 && mini->lexer->type == WORD)
//	ft_cd_export_unset(mini, 0);
// if fatal != -42, pass_or_jump