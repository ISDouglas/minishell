/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:43 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/01 15:16:45 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_operators(t_token *tokens, int *pipe, int *redirect)
{
	*pipe = 0;
	*redirect = 0;
	
	while (tokens)
	{
		if (tokens->type == PIPE)
			(*pipe)++;
		else if (tokens->type == REDIRECT_IN || tokens->type == REDIRECT_OUT ||
				tokens->type == HEREDOC || tokens->type == APPEND)
			(*redirect)++;
		tokens = tokens->next;
	}
	if ((*pipe > 0) || (*redirect > 0))
		return (1);
	return (0);
}

int	ft_execute_cmd(t_token *tokens, t_mini *mini)
{
	char	*path;
	pid_t	pid;
	t_token	*current;
	int		i;
	char	**cmd;
	int		isbuilt;
	int 	end;
	int 	start;

	start = 0;
	end = 0;
	i = 0;
	current = tokens;
	if (mini->array_env)
		ft_free_array(mini->array_env);
	mini->array_env = ft_env_to_array(mini->env);
	isbuilt = ft_isbuilt_in(tokens->value, tokens, mini);
	if (isbuilt == EXIT_CMD)
		return (ft_free_mini(mini), EXIT_CMD);
	else if (isbuilt == NOT_BUILT_IN_CMD)
	{
		//***
		if (tokens->value[0] == '/' || (tokens->value[0] == '.'
			&& tokens->value[1] == '/'))
			path =ft_strdup(tokens->value);
		else
		//***
			path = ft_get_path(tokens->value, mini->env);
		if (!path)
		{
			perror("Command not found");
			status = 127;
			return (status);
		}
		while (current && current->type == WORD)
		{
			i++;
			current = current->next;
		}
		cmd = malloc(sizeof(char *) * (i + 1));
		if (!cmd)
			return (perror("command malloc failed\n"), 1);
		i = 0;
		while (tokens && tokens->type == WORD)
		{
			cmd[i++] = strdup(tokens->value);
			tokens = tokens->next;
		}
		cmd[i] = NULL;
		pid = fork();
		if (pid == 0)
		{
			execve(path, cmd, mini->array_env);
			perror("execve");
			status  = 1;
			ft_free_mini(mini);
			exit(status);
		}
		else if (pid < 0)
		{
			perror("fork");
			status  = 1;
			ft_free_mini(mini);
			exit(status);
		}
		waitpid(pid, &status, 0);
		status = (status >> 8) & 0xFF;
		printf("status: %d\n", status);
		free(path);
		ft_free_array(cmd);
	}
	return (0);
}

int	ft_execute(t_mini *mini)
{
	t_token	*current;
	int	nb_pipe;
	int	nb_redirect;

	current = mini->lexer;
	if (!current)
		return (1);
	while (current)
	{
		if (current->type == WORD &&
			!ft_count_operators(mini->lexer, &nb_pipe, &nb_redirect))
			return (ft_execute_cmd(current, mini));
		current = current->next;
	}
	return (0);
}
