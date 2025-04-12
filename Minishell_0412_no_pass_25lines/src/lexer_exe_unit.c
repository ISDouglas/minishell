/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_exe_unit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:35:54 by layang            #+#    #+#             */
/*   Updated: 2025/03/31 14:36:23 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_unit(t_mini	*mini)
{
	int i;
	t_token	*tmp;
	int		syntax_err;       // ** test code

	dup2(mini->stdout_fd, 1);  // ** test code
	syntax_err = ft_syntax_err_ctr(mini->lexer); // ** test code
	dup2(mini->log_fd, 1);     // ** test code
	if (syntax_err == -1)
		return (-1);
	i = 1;
	tmp = mini->lexer;
	while (tmp)
	{
		if (tmp->type == AND || tmp->type == PIPE || tmp->type == OR)
			if (tmp->type == PIPE)
				i++;
		tmp = tmp->next;
	}
	printf("\n*nb of unit: %d\n", i);                // ** test code
	return (i);
}

void ft_fill_tab(t_mini *mini)
{
	int i;
	t_token	*tmp;
	t_token *new_token;

	mini->exe_tab = malloc(sizeof(t_token	*) * mini->tab_size);
	if (!mini->exe_tab)
		return ;
	i = 0;
	tmp = mini->lexer;
	while (i < mini->tab_size)
	{
		mini->exe_tab[i] = NULL;	
		while (tmp && tmp->type != PIPE)
		{		
			new_token = ft_create_token(tmp->value, tmp->type);
            ft_add_token(&mini->exe_tab[i], new_token);
			printf("**Filling tab: add line %d: %s\n\n", i, tmp->value);
			tmp = tmp->next;	
		}
		if (tmp && tmp->type == PIPE)
			tmp = tmp->next;
		i++;
	}
	printf("\nUntil line %d, finish fill_tab. Executing>>>>-------\n", i);             
}
