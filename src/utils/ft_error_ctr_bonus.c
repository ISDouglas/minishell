/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_ctr_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:52:36 by layang            #+#    #+#             */
/*   Updated: 2025/05/10 16:58:56 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	print_syntax_error(char *msg)
{
	ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
	ft_putstr_fd(msg, 2);
	write(2, "'\n", 2);
	(void)ft_link_status(NULL, 2);
	return (-1);
}

static int	ft_syntax_err_ctr_type(t_token *curr)
{
	if (ft_is_operator(curr) && ft_is_operator(curr->next))
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

static int	check_open_quote(char	*input)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		input++;
	}
	if (in_single_quote || in_double_quote)
	{
		if (in_single_quote)
			return (1);
		else
			return (2);
	}
	return (0);
}

int	ft_open_quote(t_mini	*mini)
{
	int	check;

	check = check_open_quote(mini->input);
	if (check == 0)
		return (0);
	dup2(mini->stdout_fd, 1);
	if (check == 1)
	{
		printf("Minishell : single quote not closed.\n");
		(void)ft_link_status(NULL, 0);
	}
	if (check == 2)
	{
		printf("Minishell : double quote not closed.\n");
		(void)ft_link_status(NULL, 0);
	}
	dup2(mini->log_fd, 1);
	return (1);
}

int	ft_syntax_err_ctr(t_token *lexer)
{
	t_token	*curr;

	curr = lexer;
	if (!curr)
		return (0);
	if (ft_is_operator(curr))
		return (print_syntax_error(curr->value));
	while (curr && curr->next)
	{
		if (ft_syntax_err_ctr_type(curr))
			return (-1);
		curr = curr->next;
	}
	if (ft_is_operator(curr) || curr->type == REDIRECT_IN
		|| curr->type == REDIRECT_OUT || curr->type == HEREDOC
		|| curr->type == APPEND)
		return (print_syntax_error("newline"));
	return (0);
}
