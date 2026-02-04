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
		printf("   in: %d, out: %d cmd: %s par_n: %d\n", c->infile,
			c->outfile, c->cmd, c->par_n);
		printf("start index: %zu\n", c->start);
		c = c->next;
		i++;
	}
}

void	ft_print_cmdarray(char	**cmds)
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		printf("\n          *cmd %d: %s\n", i, cmds[i]);
		i++;
	}
}

void	ft_test_log(t_mini	*mini)
{
	char	*pwd;

	pwd = ft_getenv(mini->env, "PWD");
	pwd = ft_strjoin(pwd, "/log_tmp");
	mini->log_fd = open(pwd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	free(pwd);
}

/* void	ft_log_print(t_mini	*mini, char	*title, char	*s)
{
	if (!s)
		return;
	ft_putstr_fd(title, mini->log_fd);
	ft_putstr_fd(" ", mini->log_fd);
	ft_putstr_fd(s, mini->log_fd);
	ft_putstr_fd("\n", mini->log_fd);
}

give up fork in subshell
in subshell_tmp
each time when layer > 1:
subshell_fd = open("subshell_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
close when free(mini, 1);
each time execution in par_n > 0, dup2(subshell_tmp)
-------------------------
1.
if pipeline reread, when 1->0 pipe: 0 reread in env_1
A: 1st pipe cmd: 
reread in env itself
B: from 2nd pipe cmd: when cmd after pipe: chose the hihgest one
case 1|0   0 reread in env_1 
case 0|0   0 reread in env_0
case 0|1   1 reread in env_1
-------------------------
2.
determine the env to use before make env_array

3:
when to pass/change env:
before ft_is_built_in
before ft_not_built_in
 */