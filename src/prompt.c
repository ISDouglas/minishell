/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:50:54 by layang            #+#    #+#             */
/*   Updated: 2025/03/16 13:30:13 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay(); 
}


int	main(int argc, char	**argv, char	**envp)
{
    char *input;

    (void)argc;
    (void)argv;
	signal(SIGQUIT, SIG_IGN);         // ctrl-\" 
	signal(SIGINT, handle_sigint);    // ctrl-C
	
	while (1)
    {
		input = readline("minishell$ ");
		if (!input)                   // ctrl-D
			break;
/*      Control on history. */           
//       if (input[0])
//          add_history(input);
/*      Parse the command. */
//       parse_cmd(input[0]);
		free(input);
	}
	return (0);
}

// To compile
// cc prompt.c -o minishell -lreadline