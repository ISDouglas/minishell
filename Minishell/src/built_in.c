/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:30:21 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/26 09:51:24 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
	}
	else
		perror("pwd");
	return (SUCCESS);
}

int	ft_echo(t_token *tokens)
{
	int	newline;

	newline = 1;
	if (tokens->next == NULL)
	{
		write(1, "\n", 1);
		return (SUCCESS);
	}
	if (tokens->next && ft_strncmp(tokens->next->value, "-n", 2) == 0)
	{
		newline = 0;
		tokens = tokens->next;
	}
	else
		tokens = tokens->next;
	while (tokens && tokens->type == WORD)
	{
		write(1, tokens->value, ft_strlen(tokens->value));
		if (tokens->next && tokens->next->type == WORD)
			write(1, " ", 1);
		tokens = tokens->next;
	}
	if (newline)
		write(1, "\n", 1);
	else
		write(1, "%\n", 2);
	return (SUCCESS);
}

int	ft_cd(t_token *tokens)
{
	char	*path;

	if (tokens->next == NULL)
	{
		chdir(getenv("HOME"));
		return (SUCCESS);
	}
	if (strcmp(tokens->next->value, "~") == 0)
	{
		chdir(getenv("OLDPWD"));
		return (SUCCESS);
	}
	path = tokens->next->value;
	if (chdir(path) != 0)
	{
		perror("cd");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	ft_isbuilt_in(char *cmd, t_token *tokens, t_env *env)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (ft_echo(tokens));
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (ft_cd(tokens));
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return(ft_export(tokens, &env));
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (ft_unset(tokens, env));
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (ft_get_env(env));
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		exit(0);
	return (FAILURE);
}
