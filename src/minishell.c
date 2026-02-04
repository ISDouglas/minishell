/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:44:04 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/10 14:40:10 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_start_animation(void)
{
	char	*logo[6];
	int		i;

	logo[0] = "███    ███ ██ ███    ██ ██ ██████ ██  ██ ██████ ██     ██    \n";
	logo[1] = "████  ████ ██ ████   ██ ██ ██     ██  ██ ██     ██     ██    \n";
	logo[2] = "██ ████ ██ ██ ██ ██  ██ ██ ██████ ██████ █████  ██     ██    \n";
	logo[3] = "██  ██  ██ ██ ██  ██ ██ ██     ██ ██  ██ ██     ██     ██    \n";
	logo[4] = "██      ██ ██ ██   ████ ██ ██████ ██  ██ ██████ ██████ ██████\n";
	logo[5] = NULL;
	i = 0;
	write(1, "\n", 1);
	while (logo[i] != NULL)
	{
		write(1, "\033[1;32m", 7);
		write(1, logo[i], ft_strlen(logo[i]));
		i++;
	}
	write(1, "\n", 1);
	write(1, "                                    by Layang and Nimorel\n", 59);
	write(1, "\033[0m", 4);
	write(1, "\n", 1);
}

void	ft_init_mini(t_mini	*mini, char **envp)
{
	(void)ft_link_status(NULL, 0);
	mini->input = NULL;
	mini->lexer = NULL;
	mini->exe_tab = NULL;
	mini->tab_size = 0;
	mini->env = ft_init_env(envp);
	mini->sub_env = NULL;
	mini->layer = 0;
	mini->array_env = NULL;
	mini->cmd_array = NULL;
	mini->pre = -1;
	mini->cpid = NULL;
	mini->log_fd = -1;
	mini->stdout_fd = dup(STDOUT_FILENO);
	ft_test_log(mini);
}

static void	ft_prompt(t_mini	*mini)
{
	while (1)
	{
		mini->pre = -1;
		dup2(mini->stdout_fd, STDOUT_FILENO);
		mini->input = readline(PURPLEB "minishell" X YELLOW "$ " X);
		if (!mini->input)
			break ;
		if (mini->input[0])
		{
			dup2(mini->log_fd, 1);
			printf("************ ************ ************\n");
			printf("\n**get input: %s\n", mini->input);
			add_history(mini->input);
			if (ft_is_empty_input(mini->input) || ft_open_quote(mini)
				|| ft_lexer(mini) == 2)
			{
				ft_free_mini(mini, 0);
				continue ;
			}
			if (mini->exe_tab && mini->env)
				ft_execute(mini);
			ft_free_mini(mini, 0);
		}
	}
}

int	main(int argc, char	**argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	if (!*envp)
		return (printf("Minishell: minishell need envp.\n"), 0);
	ft_start_animation();
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	ft_init_mini(&mini, envp);
	ft_prompt(&mini);
	ft_free_mini(&mini, 1);
	return (0);
}
