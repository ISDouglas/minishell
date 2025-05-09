/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:36:39 by layang            #+#    #+#             */
/*   Updated: 2025/05/08 09:39:12 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

static void	ft_get_cmd_line(t_mini	*mini, t_token	**tokens)
{
	t_token	*cur;
	int		i;

	i = 0;
	cur = *tokens;
	while (cur)
	{
		if (cur->cmd)
		{
			mini->cmd_array[i++] = ft_strdup(cur->cmd);
			printf("\n**get cmd: %s\n", cur->cmd);
		}
		cur = cur->next;
	}
	mini->cmd_array[i] = NULL;
}

void	ft_fill_cmd(t_token **tokens, t_mini *mini)
{
	t_token	*cur;

	cur = *tokens;
	printf("\nstart fill cmd: \n");
	while (cur)
	{
		if (cur->type == REDIRECT_IN || cur->type == REDIRECT_OUT
			|| cur->type == APPEND)
			ft_shift_in_out(&cur, cur->type, mini);
		else if (cur->type == HEREDOC)
			ft_here_doc(&cur, mini);
		else
			ft_add_cmd(mini, &cur, cur->type);
	}
	if (ft_malloc_array(tokens, mini) == -1)
		ft_file_ctr(-1, 0, NULL, mini);
	ft_get_cmd_line(mini, tokens);
}

static void	ft_not_built_in(t_mini *mini, t_env	**env)
{
	char	*path;

	if (!mini->cmd_array[0])
	{
		ft_free_mini(mini, 1);
		exit(0);
	}
	path = ft_get_path(mini->cmd_array[0], mini->env);
	if (!path)
		path = ft_strdup(mini->cmd_array[0]);
	path = ft_check_path_validity(mini, path);
	if (mini->array_env)
		ft_free_array(&mini->array_env);
	mini->array_env = ft_env_to_array(*env);
	if (execve(path, mini->cmd_array, mini->array_env) == -1)
	{
		perror("execve");
		free(path);
		ft_free_mini(mini, 1);
		exit(1);
	}
}

void	ft_exe_cmd(t_mini *mini, int i, t_env	**env)
{
	int	is;
	int	ex;

	is = ft_cmd_type(mini->cmd_array[0]);
	if (is == BUILT_IN_CMD)
	{
		if (count_layer(mini, i) > 0 && ft_strcmp(mini->cmd_array[0],
				"cd") == 0 && !((i - 2 >= 0 && next_type(mini, i - 2) == PIPE)
				|| next_type(mini, i) == PIPE))
			ex = cd_in_subshell(mini->exe_tab[i], *env);
		else
			ex = ft_is_built_in(mini->cmd_array[0], mini->exe_tab[i],
					mini, env);
		ft_free_mini(mini, 1);
		exit(ex);
	}
	else if (is == EXIT_CMD)
	{
		ex = ft_exit(mini, i);
		ft_free_mini(mini, 1);
		exit(ex);
	}
	else
		ft_not_built_in(mini, env);
}
