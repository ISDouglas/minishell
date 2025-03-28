/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel <marvin@42.fr> >          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:39:53 by nimorel           #+#    #+#             */
/*   Updated: 2025/03/27 18:00:49 by nimorel          ###   ########.fr       */
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
# include <limits.h>
//# include <wait.h>

/*****************************************************************************
 *  
 *  					MINISHELL define
 *  
 *****************************************************************************/
# define SUCCESS 0
# define FAILURE 1
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
	ENV_VAR,
	AND,
	OR,
	WILDCARDS
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

/* execute.c */
int	ft_execute(t_token *tokens, t_env *env, int *status);
int	ft_execute_cmd(t_token *tokens, t_env *env, int *status);
int	ft_count_operators(t_token *tokens, int *pipe, int *redirect);

/* execute_utils.c */
char	**ft_env_to_array(t_env *env);
void	ft_free_array(char **paths);
char	*ft_get_path_from_env(t_env *env);
char	*ft_get_path(char *cmd, t_env *env);

/* built_in*/
int		ft_isbuilt_in(char *cmd, t_token *tokens, t_env *env);
int		ft_env(t_env *env);

/* export.c */
int		ft_export(t_token *tokens, t_env **env);
int		ft_update_var(t_env *env, char *name, char *value);

/* cd.c */
int		ft_cd(t_token *tokens, t_env *env);
int		ft_pwd(void);

/* echo.c */
int		ft_echo(t_token *tokens);

/* unset */
int		ft_unset(t_token *tokens, t_env **env);

#endif