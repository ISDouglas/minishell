/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:33:31 by layang            #+#    #+#             */
/*   Updated: 2025/04/12 09:15:15 by layang           ###   ########.fr       */
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

/* for (i = 0; i < n; i++)
{
    // 只有在不是最后一个命令时才创建管道
    if (i != n - 1)
        pipe(new_pipe);   // 为中间命令创建管道

    pid = fork();
    if (pid == 0)
    {
        // 将上一个命令的输出作为当前命令的输入
        dup2(prev_read_fd, 0);   // 从上一个命令的输出

        // 如果不是最后一个命令，则输出到下一个命令的输入
        if (i != n - 1)
            dup2(new_pipe[1], 1);  // 输出到下一个命令的输入

        // 关闭多余的文件描述符
        close(new_pipe[0]);
        execve(cmd[i], ...);  // 执行当前命令
    }

    // 父进程关闭不再使用的文件描述符
    close(prev_read_fd);
    if (i != n - 1)
        close(new_pipe[1]);  // 关闭当前管道的写端

    // 设置下一轮的输入
    prev_read_fd = new_pipe[0]; // 下一轮的输入是当前管道的读端
} */
