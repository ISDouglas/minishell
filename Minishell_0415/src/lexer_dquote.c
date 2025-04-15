/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_dquote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:10:26 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/15 17:11:50 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_expand_env_vars(const char *input, t_env *env)
{
	char	*result;
	int		i;

	i = 0;
	result = malloc(1);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (input[i])
	{
		if (input[i] == '$')
			result = ft_env_process(input, &i, result, env);
		else
			result = ft_text_process(input, &i, result);
		if (!result)
			return (NULL);
	}
	return (result);
}

void	ft_handle_dquote(const char *input, size_t *i, t_token **tokens,
	t_env **env)
{
	int		start;
	int		len;
	char	*quoted_word;
	char	*expanded;

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
		expanded = ft_expand_env_vars(quoted_word, *env);
		ft_add_token(tokens, ft_create_token(expanded, WORD));
		free(quoted_word);
		free(expanded);
		(*i)++;
	}
	else
		perror("Error : double quote not closed.\n");
}
