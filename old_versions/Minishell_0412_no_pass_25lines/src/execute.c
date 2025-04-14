/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:43 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/12 09:12:10 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_simple_cmd(t_mini *mini, int i)
{
	t_token	*cur;

	printf("\n**start fill simple cmd_array\n"); // ** test code
	ft_fill_cmd(&mini->exe_tab[i], mini);
	dup2(mini->stdout_fd, STDOUT_FILENO);   // ** test code
	//ft_print_token(mini->exe_tab[i]);
	if (mini->cmd_array)
	{
		cur = mini->exe_tab[i];
		while (cur->next)
			cur = cur->next;
		dup2(cur->infile, 0);
		dup2(cur->outfile, 1);
		close(mini->log_fd);		// **for testing log				
		ft_exe_cmd(mini, i);
	}
}

void	ft_execute_parent(t_mini *mini, int i, int pipe[2], int pid)
{
	if (mini->tab_size != 1 && i != mini->tab_size - 1)
	{
		printf("          pipe[1] before close: %d\n", pipe[1]);     // ** test code
		close(pipe[1]);
	}
	waitpid(pid, &g_status, 0);
	//g_status = (g_status >> 8) & 0xFF;
	g_status = WEXITSTATUS(g_status);
	//printf("g_status of cmd %d: %d\n", i + 1, g_status);  // ** test code
	ft_cd_export_unset(mini, i);
	if (mini->pre != -1)
	{
		printf("          pre before close: %d\n", mini->pre);     // ** test code
		close(mini->pre); //pipe_fd_last[0]
	}
	if (mini->tab_size != 1 && i != mini->tab_size - 1)
	{
		printf("pre (before assign): %d \n", mini->pre);// ** test code
		mini->pre = pipe[0];  // renew pre
	}
	if (mini->tab_size > 1 && i != mini->tab_size - 1) // ** test code
		printf("pre(after assign) = %d, pipe[0] = %d \n", mini->pre, pipe[0]);// ** test code
}

static void	ft_handle_child(t_mini *mini, int i, int *pipe_fd)
{
	if (mini->tab_size == 1)
		ft_execute_simple_cmd(mini, i);
	else if (i != mini->tab_size - 1)
		ft_execute_child(mini, i, pipe_fd);
	ft_execute_last(mini, i);
}

//$ls /nonexistentfolder > output.txt
//ls: cannot access '/nonexistentfolder': No such file or directory
//$ chmod -w output.txt 
//$ ls /nonexistentfolder > output.txt
//bash: output.txt: Permission denied
void	ft_execute_unit(t_mini *mini, int i)
{
	int		pipe_fd[2];
	int		pid;
	
	if (mini->tab_size != 1 && i != mini->tab_size - 1)
	{
		if (pipe(pipe_fd) == -1)
		{
			ft_set_g_status("pipe", 1);
			return ;
		}
		printf("\n   ##(pipe0: %d, pipe1: %d)##\n", pipe_fd[0], pipe_fd[1]);
	}
	pid = fork();
	if (pid < 0)
	{
		ft_set_g_status("fork", 1);
		return ;
	}
	if (pid == 0)
		ft_handle_child(mini, i, pipe_fd);
	ft_execute_parent(mini, i, pipe_fd, pid);
}

int	ft_execute(t_mini *mini)
{
	int		i;

	i = 0;
	while (i < mini->tab_size)
	{
		ft_execute_unit(mini, i);
		dup2(mini->log_fd, 1);                    // ** for testing log
		printf("   g_status **: %d\n", g_status); // ** test code	
		//real shell dont care the exit status in the pipe
		//if (g_status == 1 || g_status == 2) 
		//	break ;
		i++;
	}
	//printf("\n!mini->sign = %d\n", mini->sign);
	return (g_status);                            //final g_status value
}


// 判断下一个命令是否执行if (i + 1 < mini->tab_size)
//	i++;

//add || or && 
/* i = 0;
while (i < mini->tab_size)
{
    pid = fork();
    if (pid == 0)
        exec_cmd(mini->cmd[i]); // 执行当前命令

    waitpid(pid, &g_status, 0);
    exit_status = WEXITSTATUS(g_status);

    // 保存上一次退出码（如果你需要$?）
    mini->last_exit_status = exit_status;

    // 判断下一个命令是否执行
    if (i + 1 < mini->tab_size)
    {
        if (mini->cmd[i + 1].type == CMD_AND && exit_status != 0)
            i += 2; // 跳过下一个命令
        else if (mini->cmd[i + 1].type == CMD_OR && exit_status == 0)
            i += 2; // 跳过下一个命令
        else
            i++;
    }
    else
        i++;
}
 */

