/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 08:47:36 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/05 15:15:12 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_handle_operator(const char *input, size_t *i, t_token **tokens)
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

static void	ft_handle_env_var(const char *input, size_t *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*word;

	start = (*i)++;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>", input[*i]))
		(*i)++;
	len = *i - start;
	word = malloc(len + 1);
	if (!word)
		return ;
	ft_memcpy(word, &input[start], len);
	word[len] = '\0';
	ft_add_token(tokens, ft_create_token(word, ENV_VAR));
	free(word);
}

static void	ft_handle_word(const char *input, size_t *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>", input[*i]))
		(*i)++;
	len = *i - start;
	word = malloc(len + 1);
	if (!word)
		return ;
	ft_memcpy(word, &input[start], len);
	word[len] = '\0';
	ft_add_token(tokens, ft_create_token(word, WORD));
	free(word);
}

static void	ft_handle_quote(const char *input, size_t *i, t_token **tokens)
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
		ft_add_token(tokens, ft_create_token(quoted_word, WORD));
		free(quoted_word);
		(*i)++;
	}
	else
		perror("Error : quote not closed.\n");

}

static void	ft_handle_dquote(const char *input, size_t *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*quoted_word;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] == '"')
	{
		len = *i - start;
		quoted_word = malloc(len + 1);
		if (!quoted_word)
			return ;
		ft_memcpy(quoted_word, &input[start], len);
		quoted_word[len] = '\0';
		if ((ft_strchr(quoted_word, '$') && !ft_strchr(quoted_word, '\'')))
			ft_add_token(tokens, ft_create_token(quoted_word, ENV_VAR));
		else
			ft_add_token(tokens, ft_create_token(quoted_word, WORD));
		free(quoted_word);
		(*i)++;
	}
	else
		perror("Error : double quote not closed.\n");
}

void	ft_lexer(const char *input, t_mini	*mini)
{
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (ft_strchr("|<>&", input[i]))
			ft_handle_operator(input, &i, &mini->lexer);
		else if (input[i] == '\'')
			ft_handle_quote(input, &i, &mini->lexer);
		else if (input[i] == '"')
			ft_handle_dquote(input, &i, &mini->lexer);
		else if (input[i] == '$')
			ft_handle_env_var(input, &i, &mini->lexer);
		else
			ft_handle_word(input, &i, &mini->lexer);
	}
}
