/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:44:04 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/30 09:23:18 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int status;

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
}

/* static void test_print_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		printf("%s ====== ", tmp->name);
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
} */

int	main(int argc, char	**argv, char **envp)
{
	char	*input;
	t_mini	mini;
	
	(void)argc;
	(void)argv;
	status = 0;
	ft_start_animation();
	signal(SIGINT, ft_handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	mini.env = ft_init_env(envp);
	mini.lexer = NULL;
	mini.array_env = NULL;
	while (1)
	{
		input = readline(PURPLEB "minishell" X YELLOW "$ " X);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			ft_lexer(input, &mini);
			if (mini.lexer && mini.env)
				if(ft_execute(&mini) == EXIT_CMD)
				{
					free(input);
					input = NULL;
					clear_history();
					printf("status in exit: %d\n", status);
					exit(status);
				}
			ft_free_tokens(mini.lexer);
			mini.lexer = NULL;
			free(input);
			input = NULL;
		}
	}
	ft_free_mini(&mini);
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