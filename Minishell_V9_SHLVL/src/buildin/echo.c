/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:33:18 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/09 19:33:37 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_n_flag(char **cmd_array, int *i)
{
	int	j;
	int	newline;

	newline = 1;
	while (cmd_array[*i] && cmd_array[*i][0] == '-' && cmd_array[*i][1] == 'n')
	{
		j = 2;
		while (cmd_array[*i][j] == 'n')
			j++;
		if (cmd_array[*i][j] != '\0')
			break ;
		newline = 0;
		(*i)++;
	}
	return (newline);
}

int	ft_echo(char **cmd_array, t_mini *mini)
{
	int	i;
	int	newline;

	(void) mini;
	i = 1;
	newline = ft_n_flag(cmd_array, &i);
	while (cmd_array[i])
	{
		write(1, cmd_array[i], ft_strlen(cmd_array[i]));
		if (cmd_array[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	return (SUCCESS);
}
char	*ft_getenv(t_env	*env, const char	*name)
{
	t_env	*current;

	current = env;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
