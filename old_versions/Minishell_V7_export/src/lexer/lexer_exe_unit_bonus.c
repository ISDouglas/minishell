/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_exe_unit_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:35:54 by layang            #+#    #+#             */
/*   Updated: 2025/05/04 09:22:52 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

int	ft_count_unit(t_mini	*mini)
{
	int		i;
	t_token	*tmp;
	int		syntax_err;
	int		par;

	i = 0;
	dup2(mini->stdout_fd, 1);
	syntax_err = ft_syntax_err_ctr(mini->lexer);
	par = check_parentheses_syntax(mini->lexer);
	if (syntax_err || par)
		return (syntax_err + par);
	dup2(mini->log_fd, 1);
	if (syntax_err == -1)
		return (syntax_err);
	if (mini->lexer)
		i = 1;
	tmp = mini->lexer;
	while (tmp)
	{
		if (ft_is_operator(tmp))
			i++;
		tmp = tmp->next;
	}
	printf("\n*nb of unit: %d\n", i);
	return (i + i - 1);
}

//if (*tmp)
//	printf("* *PASS to token in tab unit: %s\n", (*tmp)->value);
static void	fill_tab_unit(t_token	**tmp, t_mini	*mini, int *i)
{
	t_token	*new_token;

	while (*tmp && !ft_is_operator(*tmp) && (*tmp)->type != BRACKET
		&& (*tmp)->type != REV_BRACKET)
	{
		new_token = ft_create_token((*tmp)->value, (*tmp)->type, (*tmp)->start);
		if (!new_token)
			return ;
		new_token->par_n = (*tmp)->par_n;
		ft_add_token(&mini->exe_tab[*i], new_token);
		printf("**Filling tab: add line %d: %s\n", *i, (*tmp)->value);
		*tmp = (*tmp)->next;
	}
	(*i)++;
}

//	printf("* *PASS to token in op : %s\n", (*tmp)->value);
static void	fill_tab_op(t_token	**tmp, t_mini	*mini, int *i)
{
	t_token	*new_token;

	new_token = ft_create_token((*tmp)->value, (*tmp)->type, (*tmp)->start);
	if (!new_token)
		return ;
	new_token->par_n = (*tmp)->par_n;
	ft_add_token(&mini->exe_tab[*i], new_token);
	printf("**Filling tab op: add line %d: %s\n", *i, (*tmp)->value);
	*tmp = (*tmp)->next;
	(*i)++;
}

//	if (tmp)
//		printf("* *PASS to token in while : %s\n", tmp->value);
void	ft_fill_tab(t_mini	*mini)
{
	int		i;
	t_token	*tmp;

	if (mini->tab_size <= 0)
		return ;
	mini->exe_tab = malloc(sizeof(t_token *) * mini->tab_size);
	if (!mini->exe_tab)
		return ;
	i = 0;
	tmp = mini->lexer;
	while (i < mini->tab_size)
	{
		mini->exe_tab[i] = NULL;
		if (tmp && (tmp->type == BRACKET || tmp->type == REV_BRACKET))
			tmp = tmp->next;
		else if (tmp && ft_is_operator(tmp))
			fill_tab_op(&tmp, mini, &i);
		else
			fill_tab_unit(&tmp, mini, &i);
	}
	printf("\nUntil line %d, finish fill_tab. Executing>>>>-------\n", i);
}

int	ft_fill_exe_tab(t_mini	*mini)
{
	mini->tab_size = ft_count_unit(mini);
	if (mini->tab_size < 0)
		return (2);
	printf("\n*Tab size: %d\n", mini->tab_size);
	ft_fill_tab(mini);
	return (0);
}
