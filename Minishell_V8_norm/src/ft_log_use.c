/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_log_use.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 21:00:59 by layang            #+#    #+#             */
/*   Updated: 2025/04/13 12:13:55 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_token(t_token	*t)
{
	t_token	*c;
	int		i;

	c = t;
	i = 1;
	while (c)
	{
		printf("\n");
		printf("++token %d: value: %s, type: %d\n", i, c->value, c->type);
		printf("   in: %d, out: %d cmd: %s\n", c->infile, c->outfile, c->cmd);
		c = c->next;
		i++;
	}
}

void	ft_print_cmdarray(char	**cmds) // ** test function
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		printf("\n          *cmd %d: %s\n", i, cmds[i]);
		i++;
	}
}

void	ft_test_log(t_mini	*mini) // **for testing log
{
	char	*pwd;

	pwd = ft_getenv(mini->env, "PWD");
	pwd = ft_strjoin(pwd, "/log_tmp");
	mini->log_fd = open(pwd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	free(pwd);
}
