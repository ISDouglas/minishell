/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:02:33 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/10 14:46:51 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_create_token(char	*value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->start = 0;
	token->par_n = 0;
	token->infile = STDIN_FILENO;
	token->outfile = STDOUT_FILENO;
	token->cmd = NULL;
	token->next = NULL;
	return (token);
}

void	ft_add_token(t_token	**tokens, t_token	*new_token)
{
	t_token	*current;

	if (!new_token)
		return ;
	new_token->next = NULL;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

t_token_type	ft_get_operator_type(char c, char next_c)
{
	if (c == '&' && next_c == '&')
		return (AND);
	if (c == '|' && next_c == '|')
		return (OR);
	if (c == '|')
		return (PIPE);
	if (c == '<' && next_c == '<')
		return (HEREDOC);
	if (c == '>' && next_c == '>')
		return (APPEND);
	if (c == '<')
		return (REDIRECT_IN);
	if (c == '>')
		return (REDIRECT_OUT);
	return (WORD);
}

int	is_delimiter(char sign, t_token	*tokens)
{
	static int	is;
	t_token		*cur;

	if (!tokens)
	{
		if (sign == 'w')
		{
			is = 0;
			return (is);
		}
		if (sign == 'c')
			return (is);
	}
	cur = tokens;
	while (cur->next)
		cur = cur->next;
	if (cur->type == HEREDOC)
		is = 1;
	else
		is = 0;
	return (is);
}
