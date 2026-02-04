/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cmd_block_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 18:58:07 by layang            #+#    #+#             */
/*   Updated: 2025/05/10 15:09:00 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

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

int	word_has_glob(char	*s_glob)
{
	int		i;

	i = 0;
	if (ft_strlen(s_glob) == 0)
		return (0);
	while (s_glob[i])
	{
		if (s_glob[i] == '1')
		{
			printf("\n  .real glob location in word: { %s }\n", s_glob);
			return (1);
		}
		i++;
	}
	return (0);
}

void	renew_sign_glob(const char	*word, char	**s_glob, int sign)
{
	size_t	l_glob;
	char	*sur;
	int		i;

	l_glob = ft_strlen(*s_glob);
	if ((ft_strlen(word) == 0 && l_glob == 0) || ft_strlen(word) <= l_glob)
		return ;
	sur = malloc(sizeof(char) * (ft_strlen(word) - l_glob + 1));
	if (!sur)
	{
		perror("sur string malloc");
		return ;
	}
	i = 0;
	while (word[l_glob + i])
	{
		if (word[l_glob + i] == '*' && sign == 1 && !is_delimiter('c', NULL))
			sur[i] = '1';
		else
			sur[i] = '0';
		i++;
	}
	sur[i] = '\0';
	*s_glob = ft_strjoin_free(*s_glob, sur);
}

//printf("\n..Got word before create token: %s\n", word);
static void	ft_block_token(char	*word, size_t j, t_mini	*mini, char	*s_glob)
{
	if (!word || (!ft_strcmp(word, "") && mini->input[j] == '$'))
	{
		if (word)
			free(word);
		return ;
	}
	if (word_has_glob(s_glob))
	{
		if (ft_get_files(word, mini, s_glob, j))
		{
			free(word);
			return ;
		}
	}
	ft_add_token(&mini->lexer, ft_create_token(word, WORD, j));
	free(word);
}

void	ft_handle_block(const char *input, size_t *i, t_mini *mini)
{
	char	*word;
	size_t	j;
	char	*sign_glob;

	j = *i;
	ft_block_init(&word, &sign_glob);
	(void)is_delimiter('w', mini->lexer);
	while (input[*i] && !ft_isspace(input[*i]) && !ft_strchr("|<>&()",
			input[*i]))
	{
		if (input[*i] == '\'')
			word = ft_strjoin_free(word, get_squote_block(input, i));
		else if (input[*i] == '"')
			word = ft_strjoin_free(word, get_dquote_block(input, i, mini));
		else
		{
			renew_sign_glob(word, &sign_glob, 0);
			word = ft_strjoin_free(word, get_word_block(input, i, mini));
			renew_sign_glob(word, &sign_glob, 1);
		}
	}
	ft_block_token(word, j, mini, sign_glob);
	free(sign_glob);
}
