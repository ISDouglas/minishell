/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:44:04 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/20 16:21:08 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

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

int	main(int argc, char	**argv, char	**envp)
{
	char	*input;
	t_token	*lexer;
	t_env	*new_env;

	(void)argc;
	(void)argv;
	(void)envp;
	ft_start_animation();
	signal(SIGINT, ft_handle_sigint);
	new_env = ft_init_env(envp);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			lexer = ft_lexer(input);
			if (lexer && new_env)
				ft_execute(lexer, new_env);
			ft_free_tokens(lexer);
			free(input);
			input = NULL;
		}
	}
	return (0);
}
/*// test of lexer
			while (lexer)
			{
				printf("Token: %s Type: %d\n", lexer->value, lexer->type);
				lexer = lexer->next;
			}
			ft_free_tokens(lexer);
			// end of test*/
/*//test env	
	while (new_env)
	{
		printf("Env: %s=%s\n", new_env->name, new_env->value);
		new_env = new_env->next;
	}
	// end of tes*/