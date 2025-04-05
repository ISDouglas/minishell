/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nimorel <nimorel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:39:53 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/05 14:02:45 by nimorel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*
Lan modify on 0328:

               Modofy code in using t_mini:
a: 
  add function void	ft_free_mini(t_mini *all) in utils.c
b: modified params in ft_execute_cmd as t_mini *mini
  int	ft_execute_cmd(t_token *tokens, t_mini *mini)
2:
  int	ft_isbuilt_in(char *cmd, t_token *tokens, t_mini *mini)
3:
  void ft_lexer(const char *input, t_mini	*mini)
--------------------------------------------------------------------------------
1: to control command "exit"
  in  minishell.h:
  # define EXIT_CMD 1000
  
  in ft_execute_cmd:
  if (isbuilt == EXIT_CMD)
		return (ft_free_mini(mini), EXIT_CMD);
		
  in minishell.c
  if(ft_execute(&mini) == EXIT_CMD)
				{
					free(input);
					input = NULL;
					clear_history();
					printf("status in exit: %d\n", status);
					exit(status);
				}

4:
  in t_env	*ft_init_env(char **envp):
			if (!new_node)
			{
				ft_free_env(head);  \\add this line
				return (NULL);
			}
5:
  add -g -O0 in CFLAGS in Makefile of libft and minishell
6:
  in buildin-cd: int	ft_cd(t_token *tokens, t_env *env)
  add 	free(oldpwd); 
		before return (SUCCESS);
7:
  Note: in the build in commands, can not use getenv()? cause 
	  it will find in the original env, not our env?
8:
  in ft_cd(), add "cd .." ? Subject: cd with only a relative or absolute path

9:
  add define color in minishell.h

10:
  modified return value of ft_isbuilt_in to be 1001, not FAILURE 1.
  cause in builtin functions (like ft_cd()), possible to return FAILURE 1 too
  so confilct to know if it is builtin cmd or not.
   (ex. on cmd "cd", will show error message)
   
11:
  add command "echo $?" in ft_echo_process
*/

/*
Nico modify on 3003:
1 : create ft_getenv in environment.c for ;odify our env
2 ; call ft_getenv in pace getenc in echo.c and cd.case
3 : modif lexer.c with ft_handle_quote for simple quote and 
		ft_handle_dquote for double quote
4 : modif echo.c now work with env var
5 : modif minishell.h for codindboth linux and macos
6 : remove last free(oldpwd) witch crash sometime
Nico modif on 0401
1 :launch executable using a relative or an absolute path
*/

/*
Nico modify 0504
1 : add ft_init_env_process for 25 lines norm in environment.c
2 : norminette modification in minishell.h remember to remove preprocessor 
	instructions for macos
3 : rename status to g_status for the global variable norm
4 : mofif export.c resolve error display in update when we add a new value
5 : norminette ok for all files except minishell.c lexer.c execute.c wait
	for Lan modif on these files
*/

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
# include <linux/limits.h>
//# include <wait.h>

// remove MacOs before push
#if __linux__
	# include <wait.h>
#endif
#if __APPLE__
	extern int rl_replace_line(const char *text, int i);
#endif
//

/*****************************************************************************
 *  
 *  					MINISHELL define
 *  
 *****************************************************************************/
# define SUCCESS 0
# define FAILURE 1
# define EXIT_CMD 1000
# define NOT_BUILT_IN_CMD 1001

# define RED		"\033[0;31m"
# define GREEN		"\033[0;32m"
# define GREENB		"\033[32;1m"
# define YELLOW		"\033[0;33m"
# define BLUE 		"\033[0;34m"
# define BLUEB 		"\033[34;1m"
# define PURPLE		"\033[0;35m"
# define PURPLEB	"\033[35;1m"
# define CYAN		"\033[0;36m"
# define BOLD		"\033[0;1m"
# define X			"\033[0m"
/*****************************************************************************
 *  
 *  					MINISHELL structures and enums
 *  
 *****************************************************************************/
typedef enum e_token_type
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

typedef struct s_token
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
}					t_env;

typedef struct s_mini
{
	t_token	*lexer;
	t_env	*env;
	char	**array_env;
}			t_mini;

extern int	g_status;

/*****************************************************************************
 *  
 *  					MINISHELL function prototypes
 *  
 *****************************************************************************/
/* minishell.c */
void			ft_handle_sigint(int sig);

/* lexer.c */
void			ft_lexer(const char *input, t_mini	*mini);

/* lexer_utils.c */
void			ft_free_tokens(t_token *tokens);
t_token			*ft_create_token(char *value, t_token_type type);
void			ft_add_token(t_token **tokens, t_token *new_token);
t_token_type	ft_get_operator_type(char c, char next_c);

/* utils.c */
int				ft_isspace(int c);
int				ft_strcmp(const char *s1, const char *s2);
void			ft_free_mini(t_mini *all);

/* environment.c */
t_env			*ft_create_env_node(const char *name, const char *value);
t_env			*ft_init_env(char **envp);
void			ft_free_env(t_env *env);
char			*ft_getenv(t_env *env, const char *name);

/* execute.c */
int				ft_execute(t_mini *mini);
int				ft_execute_cmd(t_token *tokens, t_mini *mini);
int				ft_count_operators(t_token *tokens, int *pipe, int *redirect);

/* execute_utils.c */
char			**ft_env_to_array(t_env *env);
void			ft_free_array(char **paths);
char			*ft_get_path_from_env(t_env *env);
char			*ft_get_path(char *cmd, t_env *env);

/* built_in*/
int				ft_isbuilt_in(char *cmd, t_token *tokens, t_mini *mini);
int				ft_env(t_env *env);

/* export.c */
int				ft_export(t_token *tokens, t_env **env);
int				ft_update_var(t_env *env, char *name, char *value);

/* cd.c */
int				ft_cd(t_token *tokens, t_env *env);
int				ft_pwd(void);

/* echo.c */
int				ft_echo(t_token *tokens, t_env *env);

/* unset */
int				ft_unset(t_token *tokens, t_env **env);

/* error print */
//void			*error_print(int err_type, char *param, int err);
#endif
