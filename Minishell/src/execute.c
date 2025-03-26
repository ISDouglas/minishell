/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:16:43 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/26 16:36:33 by nimorel          ###   ########.fr       */
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

int	ft_execute_cmd(t_token *tokens, t_env *env, int *status)
{
	char	*path;
	char	**env_array;
	pid_t	pid;
	t_token	*current;
	int		i;
	char	**cmd;

	i = 0;
	current = tokens;
	env_array = ft_env_to_array(env);
	if (ft_isbuilt_in(tokens->value, tokens, env) == FAILURE)
	{
		path = ft_get_path(tokens->value, env);
		if (!path)
		{
			perror("Command not found");
			*status = 127;
			return (*status);
		}
		while (current && current->type == WORD)
		{
			i++;
			current = current->next;
		}
		cmd = malloc(sizeof(char *) * (i + 1));
		if (!cmd)
			return (1);
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
			execve(path, cmd, env_array);
			perror("execve");
			exit(1);
		}
		else if (pid < 0)
			perror("fork");
		waitpid(pid, status, 0);
		*status = (*status >> 8) & 0xFF;
		printf("status: %d\n", *status);
		free(path);
		ft_free_array(env_array);
		ft_free_array(cmd);
	}
	return (0);
}

int	ft_execute(t_token *tokens, t_env *env, int *status)
{
	t_token	*current;
	int	nb_pipe;
	int	nb_redirect;

	current = tokens;
	if (!current)
		return (1);
	while (current)
	{
		if (current->type == WORD &&
			!ft_count_operators(tokens, &nb_pipe, &nb_redirect))
			return (ft_execute_cmd(current, env, status));
		current = current->next;
	}
	return (0);
}
