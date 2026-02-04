/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:02:33 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/07 10:09:18 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

void	ft_block_init(char	**word, char	**sign_glob)
{
	*word = ft_strdup("");
	if (!*word)
	{
		perror("word in lexer malloc");
		return ;
	}
	*sign_glob = ft_strdup("");
	if (!*sign_glob)
	{
		perror("sign_glob malloc");
		return ;
	}
}

t_token	*ft_create_token(char	*value, t_token_type type, size_t st)
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
	token->start = st;
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
	if (c == '(')
		return (BRACKET);
	if (c == ')')
		return (REV_BRACKET);
	return (WORD);
}

/* void	ft_stamp_token(t_token	*tokens, t_token **tab_n, int pa_n)
{
	int		par;
	t_token	*to;

	if (pa_n == -1)
	{
		par = 0;
		to = tokens;
		while (to)
		{
			if (to->type == BRACKET)
				par++;
			if (to->type == REV_BRACKET)
				par--;
			to->par_n = par;
			to = to->next;
		}
	}
	else
	{
		to = *tab_n;
		while (to)
		{
			to->par_n = pa_n;
			to = to->next;
		}
	}
} */

void	ft_stamp_token(t_token	*tokens, t_token **tab_n, int pa_n)
{
	int		par;
	t_token	*to;

	par = 0;
	if (pa_n == -1)
		to = tokens;
	else
		to = *tab_n;
	while (to)
	{
		if (pa_n == -1)
		{
			if (to->type == BRACKET)
				par++;
			else if (to->type == REV_BRACKET)
				par--;
			to->par_n = par;
		}
		else
			to->par_n = pa_n;
		to = to->next;
	}
}
