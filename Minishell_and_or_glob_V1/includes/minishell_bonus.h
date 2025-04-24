/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:39:53 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/24 12:25:11 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

/* 
For and_or_V0:
1. Create the new Makefile and minishell_bonus.

To do:
fix mem leak in cmd:
ls *le* *.txt | rev >> file_fion.txt
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
# include <limits.h>
# include <dirent.h>
# include <wait.h>

/*****************************************************************************
 *  
 *  					MINISHELL define
 *  
 *****************************************************************************/
# define SUCCESS 0
# define FAILURE 1
# define EXIT_CMD 1000
# define NOT_BUILT_IN_CMD 1001
# define BUILT_IN_CMD 1002

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
	AND,
	OR,
	WILDCARDS
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	int				infile;
	int				outfile;
	char			*cmd;
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
	char	*input;
	t_token	*lexer;
	t_token	**exe_tab;
	int		tab_size;
	t_env	*env;
	char	**array_env;
	char	**cmd_array;
	int		pre;
	int		*cpid;
	int		log_fd;
	int		stdout_fd;
}			t_mini;

//extern volatile sig_atomic_t	g_status;
/******************************************************************************
 *  
 *  				add this line otherwise error occured on MACOS
 *  
 *****************************************************************************/
/* #if __linux__
	# include <wait.h>
#endif
#if __APPLE__
	extern int rl_replace_line(const char *text, int i);
#endif */

/*****************************************************************************
 *  
 *  					MINISHELL function prototypes
 *  
 *****************************************************************************/
/*  minishell.c 5 */
void			ft_start_animation(void);
void			ft_handle_sigint(int sig);
void			ft_init_mini(t_mini	*mini, char **envp);

/*  lexer_bonus.c 4*/
int				ft_lexer(t_mini	*mini);

/* lexer_utils.c 5*/
t_token			*ft_create_token(char *value, t_token_type type);
void			ft_add_token(t_token **tokens, t_token *new_token);
t_token_type	ft_get_operator_type(char c, char next_c);
char			*ft_get_dquote(const char *d_str, size_t	len, t_env	*env);
void			ft_handle_word(const char	*input, size_t *i, t_mini *mini);

/*  lexer_exe_unit.c 5*/
int				ft_strcmp(const char *s1, const char *s2);
int				ft_count_unit(t_mini	*mini);
void			ft_fill_tab(t_mini *mini);
int				ft_fill_exe_tab(t_mini	*mini);

/* lexer_dquote.c 4*/
void			dquote_pass_dollar(const char *s, char **n, t_env *e,
					size_t *i);
void			dquote_pass_char(char	**re, char c, size_t	*i);

/* lexer_wildcard.c 4*/
void			ft_handle_wildcard(const char *input, size_t *i, t_mini *mini);

/* utils.c 4*/
void			ft_handle_sigint(int sig);
int				ft_isspace(int c);
int				ft_is_empty_input(char *input);
int				ft_link_status(char	*msg, int value);

/* free_utils.c 5*/
void			ft_free_array(char ***paths);
void			ft_free_tokens(t_token **tokens);
void			ft_close_cmd_fd(t_token	*tokens);
void			ft_free_mini(t_mini *all, int sign);

/* environment.c 5*/
t_env			*ft_create_env_node(const char *name, const char *value);
t_env			*ft_init_env(char **envp);
void			ft_free_env(t_env **env);
char			*ft_getenv(t_env *env, const char *name);

/* execute.c 5 */
void			ft_execute_simple_cmd(t_mini *mini, int i, int sign);
void			ft_execute_parent(t_mini *mini, int i, int pipe[2], int pid);
void			ft_execute_unit(t_mini *mini, int i);
void			ft_execute(t_mini *mini);

/* execute_pipe.c 5 */
void			ft_execute_last(t_mini *mini, int i);
void			ft_execute_child(t_mini *mini, int i, int pipe_fd[2]);
void			ft_signal_in_child(t_mini *mini, int status);
void			ft_wait_children(t_mini	*mini);

/* execute_cmd.c 5    */
void			ft_fill_cmd(t_token **tokens, t_mini *mini);
void			ft_exe_cmd(t_mini *mini, int i);

/* execute_fill_cmd.c 5 */
void			ft_shift_in_out(t_token	**re, t_token_type t, t_mini	*mi);
void			ft_here_doc(t_token	**heredoc, t_mini	*mi);
void			ft_add_cmd(t_mini	*mini, t_token	**cmd, t_token_type	type);

/* execute_utils.c 5*/
char			**ft_env_to_array(t_env *env);
char			*ft_get_path_from_env(t_env *env);
char			*ft_get_path(char *cmd, t_env *env);

/* execute_built_in.c 5*/
int				ft_exit(t_mini	*mini, int i);
int				ft_cmd_type(char *cmd);
int				ft_is_built_in(char *cmd, t_token *tokens, t_mini *mini);
int				ft_env(t_env *env);
void			ft_cd_export_unset(t_mini *mini, int i);

/*  export.c */
int				ft_export(t_token *tokens, t_env **env, t_mini *mini);
int				ft_update_var(t_env *env, char *name, char *value);

/* cd.c */
int				ft_cd(t_token *tokens, t_env *env);
int				ft_pwd(void);

/* echo.c */
int				ft_echo(char **cmd_array, t_mini *mini);

/* unset */
int				ft_unset(t_token *tokens, t_env **env);

/* ft_error_ctr.c 5*/
int				ft_syntax_err_ctr(t_token *lexer);
void			ft_file_ctr(int fd, int he_fd, char	*msg, t_mini	*mi);
char			*ft_check_path_validity(t_mini *mini, char *path);

/* ft_test_use.c -> test functions*/
void			ft_print_token(t_token *t);
void			ft_print_cmdarray(char **cmds);
void			ft_test_log(t_mini *mini);

#endif
