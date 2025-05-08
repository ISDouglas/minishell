/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 22:13:49 by layang            #+#    #+#             */
/*   Updated: 2025/04/24 14:57:14 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	ft_wildcard_process(const char **word, const char **str)
{
	const char	*star_index = NULL;
	const char	*match_index = NULL;

	while (**str)
	{
		if (**word == '*')
		{
			star_index = *word;
			match_index = *str;
			(*word)++;
		}
		else if (**word == **str)
		{
			(*word)++;
			(*str)++;
		}
		else if (star_index)
		{
			*word = star_index;
			*str = ++match_index;
		}
		else
			return (0);
	}
	return (1);
}

static int	ft_wildcard_match(const char *word, const char *d_name)
{
	int		i;

	i = 0;
	if (ft_wildcard_process(&word, &d_name))
	{
		while (word[i] == '*')
			i++;
		return (word[i] == '\0');
	}
	return (0);
}

static void	ft_add_word(const char *word, t_mini *mini)
{
	char	*dup;

	dup = ft_strdup(word);
	ft_add_token(&mini->lexer, ft_create_token(dup, WORD));
	free(dup);
}

static void	ft_get_files(const char *word, t_mini *mini)
{
	DIR				*dir;
	struct dirent	*dent;
	int				i;
	char			*dup;

	i = 0;
	dup = NULL;
	dir = opendir(".");
	if (!dir)
		return ;
	dent = readdir(dir);
	while (dent != NULL)
	{
		if (dent->d_name[0] != '.' && ft_wildcard_match(word, dent->d_name))
		{
			dup = ft_strdup(dent->d_name);
			ft_add_token(&mini->lexer, ft_create_token(dup, WORD));
			free(dup);
			i++;
		}
		dent = readdir(dir);
	}
	closedir(dir);
	if (i == 0)
		ft_add_word(word, mini);
}

void	ft_handle_wildcard(const char *input, size_t *i, t_mini *mini)
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
	ft_get_files(word, mini);
	free(word);
}
