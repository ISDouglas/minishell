/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:33:18 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/30 09:54:21 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_n_flag(t_token **tokens)
{
	int	newline;
	int	i;

	newline = 1;
	while (*tokens && (*tokens)->type == WORD
		&& ft_strncmp((*tokens)->value, "-n", 2) == 0)
	{
		i = 1;
		while ((*tokens)->value[i] == 'n')
			i++;
		if ((*tokens)->value[i] != '\0')
			break ;
		newline = 0;
		*tokens = (*tokens)->next;
	}
	return (newline);
}

static void	ft_echo_process(t_token *tokens, t_env *env)
{
	char	*env_value;

	while (tokens && (tokens->type == WORD || tokens->type == ENV_VAR))
	{
		//printf("token passed: %s\n", tokens->value);
		if (tokens->type == WORD)
			write(1, tokens->value, ft_strlen(tokens->value));
		else if (tokens->type == ENV_VAR)
		{
			if (strncmp(tokens->value + 1, "?", 2) == 0)
				ft_putnbr_fd(status, 1);
			else
			{
				env_value = ft_getenv(env, tokens->value + 1);
				if (env_value)
					write(1, env_value, ft_strlen(env_value));
			}
		}
		if (tokens->next && (tokens->next->type == WORD
			|| tokens->next->type == ENV_VAR))
			write(1, " ", 1);
		tokens = tokens->next;
	}
}

int	ft_echo(t_token *tokens, t_env *env)
{
	int	newline;

	tokens = tokens->next;
	newline = ft_n_flag(&tokens);
	ft_echo_process(tokens, env);
	if (newline)
		write(1, "\n", 1);
	return (SUCCESS);
}
