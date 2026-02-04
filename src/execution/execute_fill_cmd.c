/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fill_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:29:53 by layang            #+#    #+#             */
/*   Updated: 2025/05/11 14:41:25 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_shift_in_out(t_token	**re, t_token_type t, t_mini	*mi)
{
	ft_pass_in_out(re);
	*re = (*re)->next;
	if (t == REDIRECT_IN)
	{
		(*re)->infile = open((*re)->value, O_RDONLY);
		if ((*re)->infile == -1 && access((*re)->value, F_OK) == 0
			&& access((*re)->value, R_OK) == -1)
			(*re)->infile = open("/dev/null", O_RDONLY);
		ft_file_ctr((*re)->infile, 0, (*re)->value, mi);
	}
	else if (t == REDIRECT_OUT)
	{
		(*re)->outfile = open((*re)->value, O_WRONLY | O_CREAT
				| O_TRUNC, 0644);
		ft_file_ctr((*re)->outfile, 0, (*re)->value, mi);
	}
	else if (t == APPEND)
	{
		(*re)->outfile = open((*re)->value, O_WRONLY | O_CREAT
				| O_APPEND, 0644);
		ft_file_ctr((*re)->outfile, 0, (*re)->value, mi);
	}
	ft_pass_in_out(re);
	*re = (*re)->next;
}

static void	loop_here_doc(t_token	**heredoc, int heredoc_fd, t_mini	*mi)
{
	size_t	len;
	char	*line;

	while (1)
	{
		write(mi->stdout_fd, "> ", 2);
		line = get_next_line(0);
		len = ft_strlen((*heredoc)->value);
		if (ft_link_status(NULL, -1) == 130)
		{
			ft_free_mini(mi, 1);
			close(heredoc_fd);
			exit(130);
		}
		if (!line || (ft_strncmp(line, (*heredoc)->value, len) == 0
				&& line[len] == '\n'))
		{
			endby_eof_delimiter(line != NULL, line, (*heredoc)->value, mi);
			break ;
		}
		write(heredoc_fd, line, ft_strlen(line));
		free(line);
	}
}

static void	ft_do_here_doc(t_token	**heredoc, int heredoc_fd, t_mini	*mi)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_handle_sigint_2);
	loop_here_doc(heredoc, heredoc_fd, mi);
	close(heredoc_fd);
}

void	ft_here_doc(t_token	**heredoc, t_mini	*mi)
{
	int	heredoc_fd;

	if (ft_link_status(NULL, -1) == 130)
		(void)ft_link_status(NULL, 0);
	ft_pass_in_out(heredoc);
	*heredoc = (*heredoc)->next;
	heredoc_fd = open("heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ft_file_ctr(0, heredoc_fd, "heredoc_temp open", mi);
	ft_do_here_doc(heredoc, heredoc_fd, mi);
	heredoc_fd = open("heredoc_tmp", O_RDONLY);
	ft_file_ctr(0, heredoc_fd, "read heredoc_temp", mi);
	(*heredoc)->infile = heredoc_fd;
	ft_pass_in_out(heredoc);
	printf("\n**Here doc collected and saved\n");
	*heredoc = (*heredoc)->next;
}

void	ft_add_cmd(t_mini	*mini, t_token	**cmd, t_token_type	type)
{
	(void) type;
	(void) mini;
	(*cmd)->cmd = ft_strdup((*cmd)->value);
	ft_pass_in_out(cmd);
	*cmd = (*cmd)->next;
}
