/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fill_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:29:53 by layang            #+#    #+#             */
/*   Updated: 2025/04/12 09:25:52 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_pass_in_out(t_token	**token)
{
	if ((*token)->next)
	{
		(*token)->next->infile = (*token)->infile;
		(*token)->next->outfile = (*token)->outfile;
	}
}

void	ft_shift_in_out(t_token	**redir, t_token_type type)
{
	ft_pass_in_out(redir);
	*redir = (*redir)->next;
	if (type == REDIRECT_IN)
	{
		(*redir)->infile = open((*redir)->value, O_RDONLY);
		ft_file_ctr((*redir)->infile, "minishell: open failed");
		if ((*redir)->infile == -1 && access((*redir)->value, F_OK) == 0
			&& access((*redir)->value, R_OK) == -1)
				(*redir)->infile = open("/dev/null", O_RDONLY);
	}
	else if (type == REDIRECT_OUT)
	{
		(*redir)->outfile = open((*redir)->value, O_WRONLY | O_CREAT
			| O_TRUNC, 0644);
		ft_file_ctr((*redir)->outfile, "minishell: open failed");
	}
	else if (type == APPEND)
	{
		(*redir)->outfile = open((*redir)->value, O_WRONLY | O_CREAT
			| O_APPEND, 0644);
		ft_file_ctr((*redir)->outfile, "minishell: open failed");
	}
	ft_pass_in_out(redir);
	*redir = (*redir)->next;
}

static void	ft_do_here_doc(t_token	**heredoc, int heredoc_fd)
{
	size_t	len;
	char	*line;
	
	while (1)
	{
		line = get_next_line(0);
		len = ft_strlen((*heredoc)->value);
		if (!line || (ft_strncmp(line, (*heredoc)->value, len) == 0
			&& line[len] == '\n'))
		{
			if (line)
				free(line);
			get_next_line(-1);
			break ;
		}
		write(heredoc_fd, line, ft_strlen(line));
		free(line);
	}
	close(heredoc_fd);
}

//if (!isatty(STDIN_FILENO))
//	close(STDIN_FILENO);
// Heredoc 只是暂时占用 STDIN，然后 创建一个临时文件 (heredoc_tmp)
//，并在执行命令时 用这个临时文件替换标准输入。 
//if ((*heredoc)->infile == STDIN_FILENO)
//	close(STDIN_FILENO);

//close(pipe_fd[0]);
	
//here_doc need to silence sig SIGQUIT (ctrl-\)
void	ft_here_doc(t_token	**heredoc)
{
	int	heredoc_fd;

	ft_pass_in_out(heredoc);
	*heredoc = (*heredoc)->next;
	heredoc_fd = open("heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ft_file_ctr(heredoc_fd, "heredoc_temp open");
	ft_do_here_doc(heredoc, heredoc_fd);
	heredoc_fd = open("heredoc_tmp", O_RDONLY);
	ft_file_ctr(heredoc_fd, "read heredoc_temp");
	(*heredoc)->infile = heredoc_fd;
	ft_pass_in_out(heredoc);
	printf("\n**Here doc collected and saved\n");  // ** test code
	*heredoc = (*heredoc)->next;
}

void	ft_add_cmd(t_token	**cmd, t_token_type type)
{
	if (type == WORD)
		(*cmd)->cmd = ft_strdup((*cmd)->value);
	ft_pass_in_out(cmd);
	*cmd = (*cmd)->next;
}
