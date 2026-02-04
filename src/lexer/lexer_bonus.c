/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:48:30 by layang            #+#    #+#             */
/*   Updated: 2025/05/07 15:58:46 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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
				op[1]), *i));
	(*i)++;
}

char	*get_dquote_block(const char	*input, size_t *i, t_mini *mini)
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

/* char	*get_word_block(const char	*input, size_t *i, t_mini *mini)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"'
		&& !ft_strchr("|<>&()", input[*i]) && input[*i] != '\'')
		(*i)++;
	word = ft_get_dquote(input + start, *i - start, mini->env);
	return (word);
} */

char	*get_word_block(const char	*input, size_t *i, t_mini *mini)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '"'
		&& !ft_strchr("|<>&()", input[*i]) && input[*i] != '\'')
		(*i)++;
	if (input[start] == '$' && (input[*i] == '"' || input[*i] == '\'')
		&& (*i - start == 1))
		return (ft_strdup(""));
	word = ft_get_dquote(input + start, *i - start, mini->env);
	return (word);
}

int	ft_lexer(t_mini	*mini)
{
	size_t	i;

	i = 0;
	while (mini->input[i])
	{
		if (ft_isspace(mini->input[i]))
			i++;
		else if (ft_strchr("|<>&()", mini->input[i]))
			ft_handle_operator(mini->input, &i, &mini->lexer);
		else
			ft_handle_block(mini->input, &i, mini);
	}
	if (!mini->lexer)
		return ((void)ft_link_status(NULL, 1), 2);
	printf("\n**add lexer succeed:\n");
	ft_stamp_token(mini->lexer, NULL, -1);
	if (stamp_layer_env(mini) == 2)
		return (2);
	printf("\n**  lexer after stamped:\n  ");
	ft_print_token(mini->lexer);
	return (ft_fill_exe_tab(mini));
}
