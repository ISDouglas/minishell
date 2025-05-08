/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_dquote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:06:20 by layang            #+#    #+#             */
/*   Updated: 2025/05/03 03:29:12 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	extract_var_len(const char *src, char buf[255])
{
	int	i;

	i = 0;
	while (src[i] && ((src[i] >= 'A' && src[i] <= 'Z')
			|| (src[i] >= 'a' && src[i] <= 'z')
			|| (src[i] >= '0' && src[i] <= '9')
			|| src[i] == '_' || src[i] == '?'))
	{
		buf[i] = src[i];
		if (i == 0 && src[i] == '?')
		{
			i++;
			break ;
		}
		i++;
	}
	buf[i] = '\0';
	return (i);
}

void	dquote_pass_dollar(const char *s, char **re, t_env *e, size_t *i)
{
	char	var[255];
	char	*env_var;
	char	*status;
	char	*n;

	if (extract_var_len(s + *i + 1, var) == 0)
		n = ft_strjoin(*re, "$");
	else if (ft_strcmp(var, "?") == 0)
	{
		status = ft_itoa(ft_link_status(NULL, -1));
		n = ft_strjoin(*re, status);
		free(status);
	}
	else
	{
		env_var = ft_getenv(e, var);
		if (env_var)
			n = ft_strjoin(*re, env_var);
		else
			n = ft_strdup(*re);
	}
	free(*re);
	*re = n;
	*i = *i + extract_var_len(s + *i + 1, var) + 1;
}

//	printf("result after char: %s\n", *re);
void	dquote_pass_char(char	**re, char c, size_t	*i)
{
	char	tmp[2];
	char	*new_re;

	tmp[0] = c;
	tmp[1] = '\0';
	new_re = ft_strjoin(*re, tmp);
	free(*re);
	*re = new_re;
	(*i)++;
}
