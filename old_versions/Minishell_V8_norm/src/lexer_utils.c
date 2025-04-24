/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:02:33 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/22 14:53:00 by layang           ###   ########.fr       */
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

char	*ft_get_dquote(const char *d_str, size_t	len, t_env	*env)
{
	size_t	i;
	char	*result;

	i = 0;
	if (len == 0)
		return (ft_strdup(""));
	result = malloc(1);
	result[0] = '\0';
	while (i < len)
	{
		if (d_str[i] == '$')
		{
			if (len == 1)
				return (free(result), ft_strdup("$"));
			else
				dquote_pass_dollar(d_str, &result, env, &i);
		}
		else
			dquote_pass_char(&result, d_str[i], &i);
	}
	return (result);
}

void	ft_handle_word(const char	*input, size_t *i, t_mini *mini)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>", input[*i]))
		(*i)++;
	word = ft_get_dquote(input + start, *i - start, mini->env);
	if (!word || (!ft_strcmp(word, "") && input[start] == '$'))
	{
		if (word)
			free(word);
		return ;
	}
	ft_add_token(&mini->lexer, ft_create_token(word, WORD));
	free(word);
}
