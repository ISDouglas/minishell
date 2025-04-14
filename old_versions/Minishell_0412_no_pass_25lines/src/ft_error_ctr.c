/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error_ctr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:52:36 by layang            #+#    #+#             */
/*   Updated: 2025/04/12 09:19:36 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
write(2, "minishell: syntax error near unexpected token `", 47);
write(2, msg, strlen(msg));
write(2, "'\n", 2); 
*/

static int	print_syntax_error(char *msg)
{
	printf("minishell: syntax error near unexpected token %s\n", msg);
	return (-1);
}

int	ft_syntax_err_ctr(t_token *lexer)
{
	t_token	*curr;

	curr = lexer;
	if (!curr)
		return (0);
	if (curr->type == PIPE || curr->type == AND || curr->type == OR)
		return (print_syntax_error(curr->value));
	while (curr && curr->next)
	{
		if ((curr->type == PIPE || curr->type == AND || curr->type == OR)
			&& (curr->next->type == PIPE || curr->next->type == AND || curr->next->type == OR))
			return (print_syntax_error(curr->next->value));
		if ((curr->type == REDIRECT_IN || curr->type == REDIRECT_OUT
			 || curr->type == HEREDOC || curr->type == APPEND)
			&& (curr->next->type != WORD && curr->next->type != ENV_VAR))
			return (print_syntax_error(curr->next->value));
		curr = curr->next;
	}
	if (curr->type == PIPE || curr->type == AND || curr->type == OR
		|| curr->type == REDIRECT_IN || curr->type == REDIRECT_OUT
		|| curr->type == HEREDOC || curr->type == APPEND)
		return (print_syntax_error("newline"));
	return (0);
}

void ft_file_ctr(int fd, char	*msg)
{
	if (fd == -1)
	{
		perror(msg);
		exit(1);
	}
	if (fd == -2)
	{
		printf("%s\n", msg);
		get_next_line(-1);
		exit(1);
	}
}

void	ft_set_g_status(char	*msg, int value)
{
	perror(msg);
	g_status = value;
}

char	*ft_check_path_validity(t_mini *mini, char *path)
{
	if (access(path, F_OK) == -1)
	{
		printf("%s: command not found\n", mini->cmd_array[0]);
		ft_free_mini(mini, 1);
		free(path);
		exit(127);
	}
	else if (access(path, X_OK) == -1)
	{
		printf("%s: permission denied\n", mini->cmd_array[0]);
		ft_free_mini(mini, 1);
		free(path);
		exit(126);
	}
	return (path);
}
