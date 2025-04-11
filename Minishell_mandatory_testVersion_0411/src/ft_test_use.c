/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_test_use.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 21:00:59 by layang            #+#    #+#             */
/*   Updated: 2025/04/10 20:59:32 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_print_token(t_token	*t)                        //test ** function
{
	t_token	*cu;                                           // | **
	int	i;

	cu = t;                                                 // | **
	i = 1;
	while (cu)
	{
		printf("\n");
		printf("++token %d: value: %s, type: %d\n", i, cu->value, cu->type);
		printf("    in: %d, out: %d cmd: %s\n", cu->infile, cu->outfile, cu->cmd);
		cu = cu->next;
		i++;
	}
}

void	ft_print_cmdarray(char	**cmds)                 // ** test function
{
	int	i;

	i = 0;
	while (cmds[i])
	{
		printf("\n          *cmd %d: %s\n", i, cmds[i]); 
		i++;
	}
}

void    ft_test_log(t_mini  *mini)                        // **for testing log
{
    mini->log_fd = open("log_tmp", O_WRONLY | O_CREAT | O_APPEND, 0644);
	dup2(mini->log_fd, 1);                        // ** for testing log
	printf("************ ************ ************\n"); // **test code
	printf("\n**get input: %s\n", mini->input);          // **test code
}
