/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 08:47:36 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/27 18:50:40 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_handle_operator(const char *input, size_t *i, t_token **tokens)
{
	char	op[3];

	op[0] = input[*i];
	op[1] = '\0';
	op[2] = '\0';
	if ((input[*i] == '<' || input[*i] == '>' || input[*i] == '&'
			|| input[*i] == '|') && input[*i + 1] == input[*i])
	{
		op[1] = input[*i];
		(*i)++;
	}
	ft_add_token(tokens, ft_create_token(op, ft_get_operator_type(op[0],
				op[1])));
	(*i)++;
}

int	ft_lexer(t_mini	*mini)
{
	size_t	i;

	i = 0;
	while (mini->input[i])
	{
		if (ft_isspace(mini->input[i]))
			i++;
		else if (ft_strchr("|<>&", mini->input[i]))
			ft_handle_operator(mini->input, &i, &mini->lexer);
		else
			ft_handle_block(mini->input, &i, mini);
	}
	if (!mini->lexer)
		return ((void)ft_link_status(NULL, 1), 2);
	printf("\n**add lexer succeed:\n");
	ft_print_token(mini->lexer);
	return (ft_fill_exe_tab(mini));
}
