/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:39:53 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/19 18:34:12 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/******************************************************************************
 *  
 *  					MINISHELL includes
 *  
 *****************************************************************************/
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <string.h>
# include "../Libft/libft.h"

/*****************************************************************************
 *  
 *  					MINISHELL structures and enums
 *  
 *****************************************************************************/
typedef enum	e_token_type
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	HEREDOC,
	APPEND,
	ENV_VAR
}	t_token_type;

typedef struct	s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
} 					t_env;

/******************************************************************************
 *  
 *  				add this line otherwise error occured  on MACOS
 *  
 *****************************************************************************/
#if __APPLE__
extern int rl_replace_line(const char *text, int i); 
#endif
/******************************************************************************
 *  
 *  					MINISHELL global variables for signal handling
 *  
 *****************************************************************************/

extern volatile sig_atomic_t g_signal;

/*****************************************************************************
 *  
 *  					MINISHELL function prototypes
 *  
 *****************************************************************************/
/* minishell.c */
void	ft_handle_sigint(int sig);

/* lexer.c */
t_token	*ft_lexer(const char *input);

/* lexer_utils.c */
void			ft_free_tokens(t_token *tokens);
t_token			*ft_create_token(char *value, t_token_type type);
void			ft_add_token(t_token **tokens, t_token *new_token);
t_token_type	ft_get_operator_type(char c, char next_c);

/* utils.c */
int		ft_isspace(int c);
int		ft_strcmp(const char *s1, const char *s2);

/* environment.c */
t_env	*ft_create_env_node(const char *name, const char *value);
t_env	*ft_init_env(char **envp);
void	ft_free_env(t_env *env);

/* environment_utils.c */
char	*ft_get_env_value(t_env *env, const char *name);
void	ft_set_env_value(t_env **env, const char *name, const char *value);
void	ft_unset_env_value(t_env **env, const char *name);
char	*ft_expand_variable(t_env *env, const char *input);


/* execute.c */

#endif