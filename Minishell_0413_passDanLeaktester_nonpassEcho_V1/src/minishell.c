/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:44:04 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/13 12:09:03 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

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

void	ft_handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_status = 130;
}

void	ft_init_mini(t_mini	*mini, char **envp)
{
	g_status = 0;
	mini->input = NULL;
	mini->lexer = NULL;
	mini->exe_tab = NULL;
	mini->tab_size = 0;
	mini->env = ft_init_env(envp);
	mini->array_env = NULL;
	mini->cmd_array = NULL;
	mini->pre = -1;
	mini->log_fd = -1;
	mini->stdout_fd = dup(STDOUT_FILENO);
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
			ft_test_log(mini);
			add_history(mini->input);
			if (ft_lexer(mini) == 2)
			{
				ft_free_mini(mini, 0);
				close(mini->log_fd);
				continue ;
			}
			if (mini->tab_size == 1
				&& !ft_strcmp(mini->exe_tab[0]->value, "exit"))
				break ;
			if (mini->exe_tab && mini->env)
				g_status = ft_execute(mini);
			ft_free_mini(mini, 0);
		}
	}
}

int	main(int argc, char	**argv, char **envp)
{
	t_mini	mini;

	(void)argc;
	(void)argv;
	ft_start_animation();
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	ft_init_mini(&mini, envp);
	ft_prompt(&mini);
	ft_free_mini(&mini, 1);
	return (0);
}
