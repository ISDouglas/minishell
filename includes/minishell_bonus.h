/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:39:53 by nimorel           #+#    #+#             */
/*   Updated: 2025/05/11 14:42:13 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_BONUS_H
# define MINISHELL_BONUS_H

/* 
For V6_REPAIRED:
1: 
  reset mini->pre = -1 in parent_process for bonus.
2:
  reread pipe len pid in bonus. (reread_pipe_chain(t_mini	*mini, int i))
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
# include <sys/stat.h>
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
	BRACKET,
	REV_BRACKET
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	size_t			start;
	int				par_n;
	int				infile;
	int				outfile;
	char			*cmd;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	int				env_flag;
	struct s_env	*next;
}					t_env;

typedef struct s_mini
{
	char	*input;
	t_token	*lexer;
	t_token	**exe_tab;
	int		tab_size;
	t_env	*env;
	t_env	**sub_env;
	int		layer;
	char	**array_env;
	char	**cmd_array;
	int		pre;
	pid_t	*cpid;
	int		log_fd;
	int		stdout_fd;
}			t_mini;

//extern volatile sig_atomic_t	g_status;
/******************************************************************************
 *  
 *  				add this line otherwise error occured on MACOS
 *  
 *****************************************************************************/
/*#if __linux__
	# include <wait.h>
#endif
#if __APPLE__
	extern int rl_replace_line(const char *text, int i);
#endif*/

/*****************************************************************************
 *  
 *  					MINISHELL function prototypes
 *  
 *****************************************************************************/
/*  minishell_bonus.c 5 */
void			ft_start_animation(void);
void			ft_init_mini(t_mini	*mini, char **envp);

/*  lexer_bonus.c 5*/
void			ft_handle_operator(const char *input, size_t *i,
					t_token **tokens);
char			*get_dquote_block(const char	*input, size_t *i,
					t_mini *mini);
char			*ft_get_dquote(const char *d_str, size_t	len, t_env	*env);
char			*get_word_block(const char	*in, size_t *i, t_mini *mini);
int				ft_lexer(t_mini	*mini);

/* lexer_utils_bonus.c 5*/
void			ft_block_init(char	**word, char	**sign_glob);
t_token			*ft_create_token(char *value, t_token_type type, size_t st);
void			ft_add_token(t_token **tokens, t_token *new_token);
t_token_type	ft_get_operator_type(char c, char next_c);
void			ft_stamp_token(t_token	*tokens, t_token **tab_n, int pa_n);

/*  lexer_exe_unit_bonus.c 5*/
int				ft_count_unit(t_mini	*mini);
void			ft_fill_tab(t_mini	*mini);
int				ft_fill_exe_tab(t_mini	*mini);

/* lexer_dquote.c 4*/
void			dquote_pass_dollar(const char *s, char **n, t_env *e,
					size_t *i);
void			dquote_pass_char(char	**re, char c, size_t	*i);

/* lexer_cmd_block_bonus.c  5*/
char			*get_squote_block(const char *input, size_t	*i);
int				word_has_glob(char	*s_glob);
void			renew_sign_glob(const char	*word, char	**s_glob, int sign);
void			ft_handle_block(const char *input, size_t *i,
					t_mini *mini);

/* lexer_wildcard.c 4*/
int				ft_get_files(const char *word, t_mini *mini, char	*s_glob,
					size_t j);
int				reget_files(const char	*word, t_token	**tab_n,
					char	*s_glob, size_t j);

/* lexer_reread.c 5*/
void			ft_tabn_reread(t_mini	*mini, size_t st, int i);
void			reread_pipe_chain(t_mini	*mini, int i);

/* utils.c 5*/
void			ft_handle_sigint(int sig);
void			ft_handle_sigint_2(int sig);
int				ft_isspace(int c);
int				ft_is_empty_input(char *input);
int				ft_link_status(char	*msg, int value);

/* utils_bonus.c  5*/
t_env			*ft_dup_env(t_env	*env);
int				stamp_layer_env(t_mini	*mini);
int				mlc_cpid(t_mini	*mini, int size);
int				dup_sub_shell(t_mini	*mini, int i);
int				is_delimiter(char sign, t_token	*tokens);

/* execute_bonus.c 5 */
void			ft_execute_simple_cmd(t_mini	*mini, int i, int sign,
					t_env	**env);
void			ft_execute_parent(t_mini *mini, int i, int pipe[2]);
void			ft_execute_unit(t_mini	*mini, int i, int *len, int layer);
void			ft_execute(t_mini *mini);

/* execute_jump.c 5*/
int				ft_is_operator(t_token	*cur);
t_token_type	next_type(t_mini	*mini, int i);
void			pass_or_jump(t_mini	*mini, int	*i);

/* execute_pipe_bonus.c 5 */
void			ft_execute_last(t_mini *mini, int i, t_env	**env);
void			ft_execute_child(t_mini *mini, int i, int pipe_fd[2],
					t_env	**env);
void			ft_signal_in_child(t_mini *mini, int status);
void			ft_wait_bonus(t_mini	*mini, int i, int	*len);
void			ft_wait_children(t_mini	*mini, int len);

/* execute_cmd_bonus.c 5    */
void			ft_fill_cmd(t_token **tokens, t_mini *mini);
void			ft_exe_cmd(t_mini *mini, int i, t_env	**env);

/* execute_fill_cmd.c 5 */
void			ft_shift_in_out(t_token	**re, t_token_type t, t_mini	*mi);
void			ft_here_doc(t_token	**heredoc, t_mini	*mi);
void			ft_add_cmd(t_mini	*mini, t_token	**cmd, t_token_type	type);

/* execute_fill_utiles.c  2*/
void			ft_pass_in_out(t_token	**token);
int				ft_malloc_array(t_token **tokens, t_mini *mini);
void			endby_eof_delimiter(int d, char	*line, char	*de, t_mini	*mi);

/* execute_utils.c 5*/
int				ft_cmd_type(char *cmd);
char			**ft_env_to_array(t_env *env);
char			*ft_get_path_from_env(t_env *env);
char			*ft_get_path(char *cmd, t_env *env);

/* execute_built_in_bonus.c 4*/
int				ft_exit(t_mini	*mini, int i);
int				ft_env(t_env *env);
int				ft_is_built_in(char	*cmd, t_token	*tokens, t_mini	*mini,
					t_env	**env);
int				is_buildin_single(t_mini	*mini, int i);
void			ft_cd_export_unset(t_mini *mini, int i);

/*  export.c */
int				ft_export(t_token *tokens, t_env **env, t_mini *mini);
int				ft_update_var(t_env *env, char *name, char *value);
int				ft_add_var(t_env **env, char *name, char *value, int d);

/* cd_bonus.c */
int				ft_cd(t_token *tokens, t_env *env);
int				ft_pwd(void);
int				cd_in_subshell(t_token *tokens, t_env *env);

/* echo.c */
int				ft_echo(char **cmd_array, t_mini *mini);
char			*ft_getenv(t_env *env, const char *name);
int				ft_is_valid_name(const char	*name);

/* environment.c 5*/
void			ft_free_env(t_env **env);
t_env			*ft_create_env_node(const char	*name, const char	*value);
t_env			*ft_init_env(char **envp);

/* unset */
int				ft_unset(t_token *tokens, t_env **env);
void			ft_handle_valid_export(t_env	**env, char	*str, char	*name);

/* free_bonus.c */
void			ft_free_sub_env(t_env	***sub_env, int layer);
int				count_layer(t_mini	*mini, int i);
void			buildin_main_shell(char	*cmd, t_mini	*mini, int i);

/* free_utils_bonus.c 5*/
void			ft_free_array(char ***paths);
void			ft_free_tokens(t_token **tokens);
void			ft_close_cmd_fd(t_token	*tokens);
void			ft_free_mini(t_mini *all, int sign);

/* ft_error_ctr.c 5*/
int				print_syntax_error(char *msg);
int				ft_open_quote(t_mini	*mini);
int				ft_syntax_err_ctr(t_token *lexer);

/* ft_file_path_ctr.c 2*/
void			ft_file_ctr(int fd, int he_fd, char	*msg, t_mini	*mi);
char			*ft_check_path_validity(t_mini *mini, char *path);
void			safe_exit_child(t_mini	*mini, char	*msg, int code);

/* ft_syntax_err.c 5*/
int				check_parentheses_syntax(t_token	*lexer);

/* ft_log_use.c -> test functions 3*/
void			ft_print_token(t_token *t);
void			ft_print_cmdarray(char **cmds);
void			ft_test_log(t_mini *mini);

#endif
