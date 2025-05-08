/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:43 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/03 17:13:03 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	ft_execute_simple_cmd(t_mini	*mini, int i, int sign)
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
		dup2(cur->infile, 0);
		dup2(cur->outfile, 1);
		if (sign == 0)
			close(mini->log_fd);
		if (sign == 0)
			ft_exe_cmd(mini, i);
	}
}

void	ft_execute_parent(t_mini	*mini, int i, int pipe[2])
{
	if (mini->pre != -1)
	{
		printf("----------pre before close: %d\n", mini->pre);
		close(mini->pre);
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

static void	ft_handle_child(t_mini	*mini, int i, int *pipe_fd)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, ft_handle_sigint_2);
	if ((i - 2 >= 0 && next_type(mini, i - 2) == PIPE)
		|| next_type(mini, i) == PIPE)
	{
		if (next_type(mini, i) == PIPE)
			ft_execute_child(mini, i, pipe_fd);
		else
			ft_execute_last(mini, i);
	}
	else
		ft_execute_simple_cmd(mini, i, 0);
}

void	ft_execute_unit(t_mini	*mini, int i, int len)
{
	int	pipe_fd[2];
	int	pid;

	if (next_type(mini, i) == PIPE)
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
	mini->cpid[len] = pid;
	ft_execute_parent(mini, i, pipe_fd);
}

void	ft_execute(t_mini	*mini)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	mini->cpid = malloc(sizeof(int) * mini->tab_size);
	if (!mini->cpid)
	{
		(void)ft_link_status("malloc cpid failed", 1);
		return ;
	}
	while (i < mini->tab_size)
	{
		ft_execute_unit(mini, i, len);
		dup2(mini->log_fd, 1);
		ft_wait_bonus(mini, i, &len);
		if (!((i - 2 >= 0 && next_type(mini, i - 2) == PIPE)
				|| (next_type(mini, i) == PIPE)))
		{
			if (mini->exe_tab[i]->type == WORD)
				ft_cd_export_unset(mini, i);
			dup2(mini->log_fd, 1);
		}
		pass_or_jump(mini, &i);
	}
}
//if (mini->tab_size == 1 && mini->lexer->type == WORD)
//	ft_cd_export_unset(mini, 0);

/* 
void postOrderTraversal(TreeNode* node) {
    if (node == NULL) return;

    postOrderTraversal(node->left);   // 左子树
    postOrderTraversal(node->right);  // 右子树
    printf("%c\n", node->val);        // 根节点
}

*/