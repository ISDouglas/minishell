/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_ctr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:52:36 by layang            #+#    #+#             */
/*   Updated: 2025/04/22 12:14:37 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_syntax_error(char *msg)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(msg, 2);
	write(2, "'\n", 2);
	return (-1);
}

static int	ft_syntax_err_ctr_type(t_token *curr)
{
	if ((curr->type == PIPE || curr->type == AND || curr->type == OR)
		&& (curr->next->type == PIPE || curr->next->type == AND
			|| curr->next->type == OR))
		return (print_syntax_error(curr->next->value));
	if ((curr->type == REDIRECT_IN || curr->type == REDIRECT_OUT
			|| curr->type == HEREDOC || curr->type == APPEND)
		&& curr->next->type != WORD)
		return (print_syntax_error(curr->next->value));
	if ((curr->type == REDIRECT_IN || curr->type == REDIRECT_OUT
			|| curr->type == HEREDOC || curr->type == APPEND)
		&& curr->next->type != WORD)
		return (print_syntax_error(curr->next->value));
	return (0);
}

int	ft_syntax_err_ctr(t_token *lexer)
{
	t_token	*curr;

	curr = lexer;
	if (!curr)
		return (0);
	if (curr->type == PIPE || curr->type == AND || curr->type == OR)
		return (print_syntax_error(curr->value));
	while (curr && curr->next)
	{
		if (ft_syntax_err_ctr_type(curr))
			return (1);
		curr = curr->next;
	}
	if (curr->type == PIPE || curr->type == AND || curr->type == OR
		|| curr->type == REDIRECT_IN || curr->type == REDIRECT_OUT
		|| curr->type == HEREDOC || curr->type == APPEND)
		return (print_syntax_error("newline"));
	return (0);
}

void	ft_file_ctr(int fd, int he_fd, char	*msg, t_mini	*mi)
{
	if (fd == -1 || he_fd == -1)
	{
		if (msg)
			perror(msg);
		ft_free_mini(mi, 1);
		if (he_fd == -1)
			get_next_line(-1);
		exit(1);
	}
}

char	*ft_check_path_validity(t_mini *mini, char *path)
{
	if (!ft_strcmp(path, "") || access(path, F_OK) == -1)
	{
		ft_putstr_fd(mini->cmd_array[0], 2);
		ft_putstr_fd(": command not found", 2);
		write(2, "\n", 2);
		ft_free_mini(mini, 1);
		free(path);
		exit(127);
	}
	else if (access(path, X_OK) == -1)
	{
		ft_putstr_fd(mini->cmd_array[0], 2);
		ft_putstr_fd(": permission denied", 2);
		write(2, "\n", 2);
		ft_free_mini(mini, 1);
		free(path);
		exit(126);
	}
	return (path);
}
