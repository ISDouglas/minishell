/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_dquote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 21:06:20 by layang            #+#    #+#             */
/*   Updated: 2025/04/16 11:45:27 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	extract_var_len(const char *src, char buf[255])
{
	int i = 0;
	while (src[i] && ((src[i] >= 'A' && src[i] <= 'Z') ||
					  (src[i] >= 'a' && src[i] <= 'z') ||
					  (src[i] >= '0' && src[i] <= '9') ||
					  src[i] == '_' || src[i] == '?'))
	{
		buf[i] = src[i];
		i++;
	}
	buf[i] = '\0';
	return (i);
}

/* 	else if (i == len - 1)
	{
		new_re = ft_strjoin(result, "$");
		free(result);
		return (new_re);
	}
	else
	{
		len_var = extract_var_len();
		result = ft_strjoin(result, ft_getenv_dqoute());
		i = i + len_var + 1;
	}
else
{
	// 
	char tmp[2] = {d_str[i], '\0'};
	new_result = ft_strjoin(result, tmp);
	free(result);
	result = new_result;
	i++;
}
*/

char    *dquote_last_dollar(char    *re, char  *new_re)
{
	new_re = ft_strjoin(re, "$");
	free(re);
	return (new_re);
}

void	dquote_pass_dollar(const char	*s,char	**re, char	**n, t_env	*e,
	size_t	*i)
{
	char	var[255];
	int	len_var;
	char	*env_var;
	
	len_var = extract_var_len(s + *i + 1, var);
	printf("var: %s\n", var);
	if (len_var == 0)
		*n = ft_strjoin(*re, "$");
	else if(ft_strcmp(var, "?") == 0)
		*n = ft_strjoin(*re, ft_itoa(g_status));
	else
	{
		env_var = ft_getenv(e, var);
		if (env_var)
			*n = ft_strjoin(*re, env_var);
		else
			*n = ft_strdup(*re);
	}
	free(*re);
	*re = *n;
	*i = *i + len_var + 1;
}

void	dquote_pass_char(char	**re, char	**new_re, char c, size_t	*i)
{
    char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	*new_re = ft_strjoin(*re, tmp);
	free(*re);
	*re = *new_re;
	printf("result after char: %s\n", *re);
	(*i)++;
}
