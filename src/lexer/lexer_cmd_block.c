/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cmd_block.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:59:03 by layang            #+#    #+#             */
/*   Updated: 2025/05/10 12:11:34 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_squote_block(const char *input, size_t	*i)
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
			return (NULL);
		ft_memcpy(quoted_word, &input[start], len);
		quoted_word[len] = '\0';
		(*i)++;
	}
	else
		return (NULL);
	return (quoted_word);
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

static char	*get_dquote_block(const char	*input, size_t *i, t_mini *mini)
{
	size_t	start;
	char	*quoted_word;

	start = ++(*i);
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] == '"')
	{
		quoted_word = ft_get_dquote(input + start, *i - start, mini->env);
		if (!quoted_word)
			return (NULL);
		printf("double quoted_word: %s\n", quoted_word);
		(*i)++;
	}
	else
		return (NULL);
	return (quoted_word);
}

/* static char	*get_word_block(const char	*input, size_t *i, t_mini *mini)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"'
		&& !ft_strchr("|<>&", input[*i]) && input[*i] != '\'')
		(*i)++;
	word = ft_get_dquote(input + start, *i - start, mini->env);
	return (word);
} */

static char	*get_word_block(const char	*input, size_t *i, t_mini *mini)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"'
		&& !ft_strchr("|<>&", input[*i]) && input[*i] != '\'')
		(*i)++;
	if (input[start] == '$' && (input[*i] == '"' || input[*i] == '\'')
		&& (*i - start == 1))
		return (ft_strdup(""));
	word = ft_get_dquote(input + start, *i - start, mini->env);
	return (word);
}

void	ft_handle_block(const char *input, size_t *i, t_mini *mini)
{
	char	*word;
	size_t	j;

	j = *i;
	word = ft_strdup("");
	(void)is_delimiter('w', mini->lexer);
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&", input[*i]))
	{
		if (input[*i] == '\'')
			word = ft_strjoin_free(word, get_squote_block(input, i));
		else if (input[*i] == '"')
			word = ft_strjoin_free(word, get_dquote_block(input, i, mini));
		else
			word = ft_strjoin_free(word, get_word_block(input, i, mini));
	}
	if (!word || (!ft_strcmp(word, "") && input[j] == '$'))
	{
		if (word)
			free(word);
		return ;
	}
	ft_add_token(&mini->lexer, ft_create_token(word, WORD));
	free(word);
}
