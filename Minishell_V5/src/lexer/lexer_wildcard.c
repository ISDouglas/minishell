/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 22:13:49 by layang            #+#    #+#             */
/*   Updated: 2025/05/03 06:20:08 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static int	wildcard_go(const char **word, const char **str,
	const char **star_index, const char **match_index)
{
	if (**word == **str)
	{
		(*word)++;
		(*str)++;
	}
	else if (*star_index)
	{
		*word = *star_index + 1;
		(*match_index)++;
		*str = *match_index;
	}
	else
		return (0);
	return (1);
}

//printf("DEBUG in *:i: %d, word=%s, str=%s\n", i, *word, *str);

static int	ft_wildcard_process(const char **word, const char **str,
	char	*s_glob)
{
	const char	*star_index = NULL;
	const char	*match_index = NULL;
	const char	*ori_word;
	int			i;

	ori_word = *word;
	while (**str)
	{
		i = *word - ori_word;
		if (**word == '*' && s_glob[i] == '1')
		{
			star_index = *word;
			match_index = *str;
			(*word)++;
		}
		else if (wildcard_go(word, str, &star_index, &match_index))
			continue ;
		else
			return (0);
	}
	return (1);
}

static int	ft_wildcard_match(const char	*word, const char	*d_name,
	char	*s_glob)
{
	int		i;
	int		z;

	i = 0;
	z = ft_strlen(word);
	if (ft_wildcard_process(&word, &d_name, s_glob))
	{
		z = z - ft_strlen(word);
		while (word[i] && word[i] == '*' && s_glob[z + i] == '1')
			i++;
		return (word[i] == '\0');
	}
	return (0);
}

int	ft_get_files(const char *word, t_mini *mini, char	*s_glob, size_t j)
{
	DIR				*dir;
	struct dirent	*dent;
	int				i;
	char			*dup;

	i = 0;
	dup = NULL;
	dir = opendir(".");
	if (!dir)
		return (printf("minishell: open dir for globbing failed.\n"), 0);
	dent = readdir(dir);
	while (dent != NULL)
	{
		if (dent->d_name[0] != '.' && ft_wildcard_match(word,
				dent->d_name, s_glob))
		{
			dup = ft_strdup(dent->d_name);
			ft_add_token(&mini->lexer, ft_create_token(dup, WORD, j));
			free(dup);
			i++;
		}
		dent = readdir(dir);
	}
	closedir(dir);
	return (i != 0);
}

int	reget_files(const char	*word, t_token	**tab_n, char	*s_glob, size_t j)
{
	DIR				*dir;
	struct dirent	*dent;
	int				i;
	char			*dup;

	i = 0;
	dup = NULL;
	dir = opendir(".");
	if (!dir)
		return (printf("minishell: open dir for globbing failed.\n"), 0);
	dent = readdir(dir);
	while (dent != NULL)
	{
		if (dent->d_name[0] != '.' && ft_wildcard_match(word,
				dent->d_name, s_glob))
		{
			dup = ft_strdup(dent->d_name);
			ft_add_token(tab_n, ft_create_token(dup, WORD, j));
			free(dup);
			i++;
		}
		dent = readdir(dir);
	}
	closedir(dir);
	return (i != 0);
}
