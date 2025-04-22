/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 09:30:21 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/09 18:05:01 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_mini	*mini, int i)
{
	unsigned char	code;

	if (mini->exe_tab[i]->next)
	{
		if (mini->exe_tab[i]->next->next)
		{
			printf("exit: too many arguments\n");
			g_status = 1;
		}	
		else
		{
			if (ft_is_numeric(mini->exe_tab[i]->next->value))
			{
				code = (unsigned char)ft_atoi(mini->exe_tab[i]->next->value);
				g_status = (unsigned int)code;
			}
			else
			{
				printf("exit: numeric argument required\n");
				g_status = 2;
			}
		}
	}
	return(g_status);
}

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
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0)
		return (BUILT_IN_CMD);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (EXIT_CMD);
	else
		return (NOT_BUILT_IN_CMD);
}

int	ft_is_built_in(char *cmd, t_token *tokens, t_mini *mini)
{
	if (ft_strcmp(cmd, "echo") == 0)
		return (ft_echo(mini->cmd_array));
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd, "cd") == 0)
		return (ft_cd(tokens, mini->env));
	else if (ft_strcmp(cmd, "export") == 0)
		return (ft_export(tokens, &mini->env));
	else if (ft_strcmp(cmd, "unset") == 0)
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
		if (ft_strcmp(cmd, "cd") == 0 || ft_strcmp(cmd, "export") == 0
			|| ft_strcmp(cmd, "unset") == 0)
				g_status = ft_is_built_in(cmd, mini->lexer, mini);
		if (ft_strcmp(cmd, "exit") == 0)
		{
			free(cmd);
			g_status = ft_exit(mini, i);
			printf("exit (with signal: %d)\n", g_status);
			ft_free_mini(mini, 1);
			exit(g_status);
		}
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