/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fill_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:29:53 by layang            #+#    #+#             */
/*   Updated: 2025/05/04 10:17:55 by layang           ###   ########.fr       */
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

static void	ft_do_here_doc(t_token	**heredoc, int heredoc_fd)
{
	size_t	len;
	char	*line;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_handle_sigint_2);
	while (1)
	{
		line = get_next_line(0);
		len = ft_strlen((*heredoc)->value);
		if (!line || (ft_strncmp(line, (*heredoc)->value, len) == 0
				&& line[len] == '\n') || ft_link_status(NULL, -1) == 130)
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

void	ft_here_doc(t_token	**heredoc, t_mini	*mi)
{
	int	heredoc_fd;

	if (ft_link_status(NULL, -1) == 130)
		(void)ft_link_status(NULL, 0);
	ft_pass_in_out(heredoc);
	*heredoc = (*heredoc)->next;
	heredoc_fd = open("heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ft_file_ctr(0, heredoc_fd, "heredoc_temp open", mi);
	ft_do_here_doc(heredoc, heredoc_fd);
	if (ft_link_status(NULL, -1) == 130)
	{
		ft_free_mini(mi, 1);
		exit(130);
	}
	heredoc_fd = open("heredoc_tmp", O_RDONLY);
	ft_file_ctr(0, heredoc_fd, "read heredoc_temp", mi);
	(*heredoc)->infile = heredoc_fd;
	ft_pass_in_out(heredoc);
	printf("\n**Here doc collected and saved\n");
	*heredoc = (*heredoc)->next;
}

/* ---------------------------------------------------------------
//0503:
static void input_here_doc(char *limiter, int *pipe_fd)
{
    char *line;
    size_t len;

    close(pipe_fd[0]);  // Close the read end of the pipe

    while (1)
    {
        // Read the next line from stdin
        line = get_next_line(0);
        if (line == NULL)
        {
// Handle EOF or read error (though this may never happen with a heredoc)
            break;
        }

        len = ft_strlen(limiter);
        
        // Check if the line matches the delimiter and ends with a newline
        if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
        {
            free(line);  // Free the line before exiting
            get_next_line(-1);  // Free the internal buffer used by get_next_line
            exit(0);  // Exit when the delimiter is encountered
        }

        // Write the line to the pipe
        ft_putstr_fd(line, pipe_fd[1]);

        // Free the line after writing to the pipe
        free(line);
    }

    // In case of any other break, make sure to clean up
    get_next_line(-1);
}

static void do_here_doc(char **delimiters, int num_delimiters, int *pipe_fd)
{
    pid_t pid;
    int i;

    // Create a pipe for each heredoc
    for (i = 0; i < num_delimiters; i++)
    {
        if (pipe(pipe_fd) == -1)
            error(1);
        
        pid = fork();
        if (pid == -1)
            error(1);
        
        if (pid == 0)
        {
            // In child process
			input_here_doc(delimiters[i], pipe_fd); 
        }
        else
        {
            // In parent process, close write end of the pipe
            close(pipe_fd[1]);
            dup2(pipe_fd[0], 0); // Redirect input to next command
            wait(NULL);
        }
    }
}*/

void	ft_add_cmd(t_mini	*mini, t_token	**cmd, t_token_type	type)
{
	(void) type;
	(void) mini;
	(*cmd)->cmd = ft_strdup((*cmd)->value);
	ft_pass_in_out(cmd);
	*cmd = (*cmd)->next;
}

/* static void	input_here_doc(char	*limiter, int pipe_fd)
{
	char	*line;
	size_t	len;

	close(pipe_fd);
	while (1)
	{
		line = get_next_line(0);
		if (!line || (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n'))
		{
			if (line)
				free(line);
			get_next_line(-1);
			break;
		}
		ft_putstr_fd(line, pipe_fd);
		free(line);
	}
}

static void	ft_do_here_doc(t_token	**heredoc, t_mini	*mi)
{
	pid_t pid;
	int pipe_fd[2];

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_handle_sigint_2);
	while (*heredoc)
	{
		if (pipe(pipe_fd) == -1)
			ft_file_ctr(1, 0, "heredoc pipe failed", mi);
		pid = fork();
		if (pid == -1)
			ft_file_ctr(1, 0, "heredoc fork failed", mi);
		if (pid == 0)
		{
			input_here_doc((*heredoc)->value, pipe_fd[1]);
			ft_file_ctr(0, 0, NULL, mi);
		}
		else
		{
			close(pipe_fd[1]);
			(*heredoc)->infile = pipe_fd[0];
			wait(NULL);
			*heredoc = (*heredoc)->next;
		}
	}
}

void	ft_here_doc(t_token	**heredoc, t_mini	*mi)
{
	if (ft_link_status(NULL, -1) == 130)
		(void)ft_link_status(NULL, 0);
	ft_pass_in_out(heredoc); 
	ft_do_here_doc(heredoc, mi);
	if (ft_link_status(NULL, -1) == 130)
	{
		ft_free_mini(mi, 1);
		exit(130);
	}
	ft_pass_in_out(heredoc);
	printf("\n**Here doc collected\n");
}
 */