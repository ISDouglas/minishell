/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:44:04 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/28 10:48:14 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int status;

void	ft_free_mini(t_mini *all)
{
	if (all->lexer)
		ft_free_tokens(all->lexer);
	if (all->env)
		ft_free_env(all->env);
	if (all->array_env)
		ft_free_array(all->array_env);
}

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
	write(1, "                                    by Layang and Nimorel\n", 62);
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
}

int	main(int argc, char	**argv, char **envp)
{
	char	*input;
	//t_token	*lexer;
	//t_env	*new_env;
	t_mini	mini;
	
	(void)argc;
	(void)argv;
	status = 0;
	ft_start_animation();
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	mini.env = malloc(sizeof(t_env));
	ft_memset(mini.env, 0, sizeof(t_env));
	mini.env = ft_init_env(envp);
	mini.lexer = malloc(sizeof(t_token));
	ft_memset(mini.lexer, 0, sizeof(t_token));
	mini.array_env = NULL;
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			mini.lexer = ft_lexer(input);
			if (mini.lexer && mini.env)
				ft_execute(&mini);
			ft_free_tokens(mini.lexer);
			free(input);
			input = NULL;
		}
	}
	clear_history();
	return (0);
}

/*
while (lexer)
			{
				printf("lexer->type = %d\n", lexer->type);
				printf("lexer->value = %s\n", lexer->value);
				lexer = lexer->next;
			}*/