/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_err.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:25:26 by layang            #+#    #+#             */
/*   Updated: 2025/05/03 07:07:06 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	print_parentheses_msg(char *msg)
{
	printf("Minishell : parentheses error near '%s'.\n", msg);
	return (-2);
}

static int	check_bracket(int prev_type, t_token *curr)
{
	if (prev_type == WORD)
		return (print_parentheses_msg(curr->value));
	if (!curr->next || ft_is_operator(curr->next)
		|| curr->next->type == REV_BRACKET)
	{
		if (!curr->next)
			return (print_parentheses_msg("newline"));
		else
			return (print_parentheses_msg(curr->next->value));
	}
	return (0);
}

static int	check_rev_bracket(int prev_type, int balance, t_token *curr)
{
	if (balance < 0)
		return (print_parentheses_msg(curr->value));
	if (prev_type == PIPE || prev_type == AND || prev_type == OR
		|| prev_type == BRACKET)
		return (print_parentheses_msg(curr->value));
	if (curr->next && curr->next->type == WORD)
		return (print_parentheses_msg(curr->next->value));
	return (0);
}

//		printf(" check type'%d' after type '%d'.\n", curr->type, prev_type);
static int	check_loop(t_token *curr, int *balance)
{
	int	prev_type;

	prev_type = -1;
	while (curr)
	{
		if (curr->type == BRACKET)
		{
			(*balance)++;
			if (check_bracket(prev_type, curr) == -2)
				return (-2);
		}
		else if (curr->type == REV_BRACKET)
		{
			(*balance)--;
			if (check_rev_bracket(prev_type, *balance, curr) == -2)
				return (-2);
		}
		if (curr->type == REV_BRACKET && curr->next
			&& curr->next->type == BRACKET)
			return (print_parentheses_msg(curr->next->value));
		prev_type = curr->type;
		curr = curr->next;
	}
	return (0);
}

// note: can not check: ((echo) "hou") 
int	check_parentheses_syntax(t_token	*lexer)
{
	int	balance;

	balance = 0;
	if (check_loop(lexer, &balance) == -2)
	{
		(void)ft_link_status(NULL, 2);
		return (-2);
	}
	if (balance != 0)
		return (print_parentheses_msg("("));
	return (0);
}
