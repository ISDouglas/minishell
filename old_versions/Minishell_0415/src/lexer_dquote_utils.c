/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_dquote_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:11:20 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/15 17:38:45 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_get_env_name(const char *input, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	len = *i - start;
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_memcpy(name, &input[start], len);
	name[len] = '\0';
	return (name);
}

static char	*ft_append_env_value(char *result, const char *value)
{
	char	*tmp;

	if (!value)
		return (result);
	tmp = ft_strjoin(result, value);
	free(result);
	return (tmp);
}

char	*ft_env_process(const char *input, int *i, char *result, t_env *env)
{
	char	*name;
	char	*value;
	char	*tmp;
	char	*code;

	(*i)++;
	if (input[*i] == '?')
	{
		code = ft_itoa(g_status);
		tmp = ft_strjoin(result, code);
		free(result);
		free(code);
		(*i)++;
		return (tmp);
	}
	name = ft_get_env_name(input, i);
	if (!name)
		return (free(result), NULL);
	value = ft_getenv(env, name);
	free(name);
	return (ft_append_env_value(result, value));
}

char	*ft_text_process(const char *input, int *i, char *result)
{
	int		start;
	int		len;
	char	*txt;
	char	*tmp;

	start = *i;
	while (input[*i] && input[*i] != '$')
		(*i)++;
	len = *i - start;
	txt = malloc(len + 1);
	if (!txt)
		return (free(result), NULL);
	ft_memcpy(txt, &input[start], len);
	txt[len] = '\0';
	tmp = ft_strjoin(result, txt);
	free(result);
	free(txt);
	return (tmp);
}
