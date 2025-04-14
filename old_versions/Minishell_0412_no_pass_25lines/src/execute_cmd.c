/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 11:36:39 by layang            #+#    #+#             */
/*   Updated: 2025/04/12 09:21:06 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_malloc_array(t_token **tokens, t_mini *mini)
{
	int		i;
	t_token	*cur;

	ft_free_array(&mini->cmd_array);
	cur = *tokens;
	i = 0;
	while (cur)
	{
		cur = cur->next;
		i++;
	}
	mini->cmd_array = malloc(sizeof(char *) * (i + 1));
	if (!mini->cmd_array)
	{
		ft_set_g_status("malloc cmd_line failed", 1);
		return (-1);
	}
	return (0);
}

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

//echo sd >b >a oo sf gg ht htyj "sf\n"
//grep sd nofile >b >a << oo
void	ft_fill_cmd(t_token **tokens, t_mini *mini)
{
	t_token	*cur;

	if (ft_malloc_array(tokens, mini) == -1)
		exit(g_status);
	cur = *tokens;
	while (cur)
	{
		if (cur->type == REDIRECT_IN || cur->type == REDIRECT_OUT
			|| cur->type == APPEND)
				ft_shift_in_out(&cur, cur->type);
		else if (cur->type == HEREDOC)
			ft_here_doc(&cur);
		else                                        //add || && () later
			ft_add_cmd(&cur, cur->type);
	}
	//printf("\n**start fill cmd_array: %d\n", i);
	ft_get_cmd_line(mini, tokens);
}


static void	ft_not_built_in(t_mini *mini)
{
	char	*path;

	path = ft_get_path(mini->cmd_array[0], mini->env);
	if (!path)
	{
		printf("Not an env PATH.\n");                          // ** test code
		path = ft_strdup(mini->cmd_array[0]);
	}
	path = ft_check_path_validity(mini, path);
    if (execve(path, mini->cmd_array, mini->array_env) == -1)
    {
		perror("execve");
        free(path);
		ft_free_mini(mini, 1);   ///add for removing mem leak
		exit(1);
	}
}

void ft_exe_cmd(t_mini *mini, int i)
{
	int	is;
	int	ex;

	//printf("\n\n<<<<**Executing result>>>>\n");
	//printf("------- ------- ------- ------- -------  -------  -------\n\n");
	is = ft_cmd_type(mini->cmd_array[0]);
	//printf("\n--->> get cmd type: %d\n", is);
	if (is == BUILT_IN_CMD)
	{
		ex = ft_is_built_in(mini->cmd_array[0], mini->exe_tab[i], mini);
		ft_free_mini(mini, 1);
		exit(ex);
	}
	else if (is == EXIT_CMD)
	{
		//printf("\nEXIT cmd recieved.\n");    // ** test code
		ft_free_mini(mini, 1);
		exit(0);
	}
	else
		ft_not_built_in(mini);
}
