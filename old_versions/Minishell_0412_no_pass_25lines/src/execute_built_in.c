/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:30:21 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/09 18:05:01 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_env *env)
{
	t_env	*current;

	if (!env)
		return (FAILURE);
	current = env;
	while (current)
	{
		if (ft_strcmp(current->value, "") != 0)
		{
			write(1, current->name, ft_strlen(current->name));
			write(1, "=", 1);
			write(1, current->value, ft_strlen(current->value));
			write(1, "\n", 1);
		}
		current = current->next;
	}
	return (SUCCESS);
}

int	ft_cmd_type(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 5) == 0 || ft_strncmp(cmd, "pwd", 4) == 0
		|| ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "export", 7) == 0
		|| ft_strncmp(cmd, "unset", 6) == 0 || ft_strncmp(cmd, "env", 4) == 0)
		return (BUILT_IN_CMD);
	else if (ft_strncmp(cmd, "exit", 5) == 0)
		return (EXIT_CMD);
	else
		return (NOT_BUILT_IN_CMD);
}

int	ft_is_built_in(char *cmd, t_token *tokens, t_mini *mini)
{
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (ft_echo(tokens, mini->env));
	else if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd, "cd", 3) == 0)
		return (ft_cd(tokens, mini->env));
	else if (ft_strncmp(cmd, "export", 7) == 0)
		return (ft_export(tokens, &mini->env));
	else if (ft_strncmp(cmd, "unset", 6) == 0)
		return (ft_unset(tokens, &mini->env));
	else
		return (ft_env(mini->env));
	//return (-1);
}

void ft_cd_export_unset(t_mini *mini, int i)
{
	char	*cmd;

	if (i == 0 && mini->tab_size == 1 && mini->lexer->type == WORD)
	{
		cmd = strdup(mini->lexer->value);
		if (ft_strncmp(cmd, "cd", 3) == 0 || ft_strncmp(cmd, "export", 7) == 0
			|| ft_strncmp(cmd, "unset", 6) == 0)
				g_status = ft_is_built_in(cmd, mini->lexer, mini);
		free(cmd);
	}
	
}

/* 
echo:
	input: token->next 
	output: write()
pwd: 
	input: NA
	output: write()
cd:
	input: NA/path(token->next)
	output: NA/err
export:
	input: NA/token->next
	output: NA/err
unset:
	input: (n, type == WORD)token->next...
	output: NA/token->next
env:
	input: NA  (no need: env -i)
	output: write()
exit:
	input: NA/token->next (ex: 127)
	output: NA/err
	*ex:
	1977  cat ttt | exit 127
 	1978  echo $?
		  127
 	1979  cat ttt | exit
 	1980  echo $?
		  0

 */