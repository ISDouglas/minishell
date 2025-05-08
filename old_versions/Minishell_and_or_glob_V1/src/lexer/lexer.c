/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 08:47:36 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/23 19:20:21 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_operator(const char *input, size_t *i, t_token **tokens)
{
	char	op[3];

	op[0] = input[*i];
	op[1] = '\0';
	op[2] = '\0';
	if ((input[*i] == '<' || input[*i] == '>' || input[*i] == '&'
			|| input[*i] == '|') && input[*i + 1] == input[*i])
	{
		op[1] = input[*i];
		(*i)++;
	}
	ft_add_token(tokens, ft_create_token(op, ft_get_operator_type(op[0],
				op[1])));
	(*i)++;
}

static void	ft_handle_quote(const char *input, size_t *i, t_mini *mini)
{
	int		start;
	int		len;
	char	*quoted_word;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
	{
		len = *i - start;
		quoted_word = malloc(len + 1);
		if (!quoted_word)
			return ;
		ft_memcpy(quoted_word, &input[start], len);
		quoted_word[len] = '\0';
		ft_add_token(&mini->lexer, ft_create_token(quoted_word, WORD));
		free(quoted_word);
		(*i)++;
	}
}

static void	ft_handle_dquote(const char *input, size_t *i, t_mini *mini)
{
	size_t	start;
	char	*quoted_word;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] == '"')
	{
		quoted_word = ft_get_dquote(input + start, *i - start, mini->env);
		printf("quoted_word: %s\n", quoted_word);
		if (!quoted_word)
			return ;
		ft_add_token(&mini->lexer, ft_create_token(quoted_word, WORD));
		free(quoted_word);
		(*i)++;
	}
}

int	ft_lexer(t_mini	*mini)
{
	size_t	i;

	i = 0;
	while (mini->input[i])
	{
		if (ft_isspace(mini->input[i]))
			i++;
		else if (ft_strchr("|<>&", mini->input[i]))
			ft_handle_operator(mini->input, &i, &mini->lexer);
		else if (mini->input[i] == '\'')
			ft_handle_quote(mini->input, &i, mini);
		else if (mini->input[i] == '"')
			ft_handle_dquote(mini->input, &i, mini);
		else
			ft_handle_word(mini->input, &i, mini);
	}
	printf("\n**add lexer succeed:\n");
	if (!mini->lexer)
		return (2);
	ft_print_token(mini->lexer);
	return (ft_fill_exe_tab(mini));
}
