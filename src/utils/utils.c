/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:57:41 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/14 12:15:42 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	(void)ft_link_status(NULL, 130);
}

/* void	ft_handle_sigint_2(int sig)
{
	(void)sig;
	(void)ft_link_status(NULL, 130);
} */

void	ft_handle_sigint_2(int sig)
{
	(void)sig;
	get_next_line(-1);
	(void)ft_link_status(NULL, 130);
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	ft_is_empty_input(char *input)
{
	while (*input)
	{
		if (!ft_isspace(*input))
			return (0);
		input++;
	}
	return (1);
}

int	ft_link_status(char	*msg, int value)
{
	static volatile sig_atomic_t	g_status;

	if (msg)
		perror(msg);
	if (value >= 0)
		g_status = value;
	return (g_status);
}
