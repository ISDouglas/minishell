/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:39:53 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/12 09:26:04 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*   
Lan modify during including Nico version 0405 on 0409:

1: change ft_free_env(t_env *env) into ft_free_env(t_env **env) to free env
   locally as used : *env = NULL; in the last line.
2: j'ai pas compris ce point
	2 : norminette modification in minishell.h remember to remove preprocessor 
	instructions for macos
3: 	add g_status = 130 in ft_handle_sigint, add exit(127)in case cmd not find,
    in execute_cmd.c.
4: add more vars in struct t_token, and so modified the create and free 
   token.
5： when check build in, change ft_strncmp(cmd, "echo", 4) == 0 into 
	ft_strncmp(cmd, "echo", 5) == 0 to avoid case like "echoo" will also work.


***(lan on 0411)TEST ON memory leaks:
1: export without =: leaks (only export with "=" will be accept by env, but not fro export)
	export hi hi= hi=hello hello
		
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
	int			infile;
	int			outfile;
	char			*cmd;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
} 					t_env;

typedef struct s_mini
{
	char	*input;
	t_token	*lexer;
	t_token **exe_tab;
	int		tab_size;
	t_env	*env;
	char 	**array_env;
	char 	**cmd_array;   // renew each time in function ft_fill_cmd
	int		pre;           // reinitiate and renew each input 
	int		log_fd;        // ** for testing log
	int		stdout_fd;     // ** for testing log
}			t_mini;        

extern int g_status;

/******************************************************************************
 *  
 *  				add this line otherwise error occured on MACOS
 *  
 *****************************************************************************/
#if __linux__
	# include <wait.h>
#endif
#if __APPLE__
	extern int rl_replace_line(const char *text, int i);
#endif

/*****************************************************************************
 *  
 *  					MINISHELL function prototypes
 *  
 *****************************************************************************/
/*  minishell.c 5 */
void			ft_start_animation(void);
void			ft_handle_sigint(int sig);
void			ft_init_mini(t_mini	*mini, char **envp);

/*  lexer.c 5*/
int 			ft_lexer(t_mini	*mini);

/* lexer_utils.c 5*/
void			ft_free_tokens(t_token **tokens);
t_token			*ft_create_token(char *value, t_token_type type);
void			ft_add_token(t_token **tokens, t_token *new_token);
t_token_type	ft_get_operator_type(char c, char next_c);
void			ft_handle_word(const char *input, size_t *i, t_token **tokens);

/*  lexer_exe_unit.c 2*/
int				ft_count_unit(t_mini	*mini);
void			ft_fill_tab(t_mini *mini);

/* utils.c 5*/
int				ft_isspace(int c);
int				ft_strcmp(const char *s1, const char *s2);
void			ft_close_cmd_fd(t_token	*tokens);
void			ft_free_mini(t_mini *all, int sign);


/* environment.c 5*/   
t_env			*ft_create_env_node(const char *name, const char *value);
t_env			*ft_init_env(char **envp);
void			ft_free_env(t_env **env);
char			*ft_getenv(t_env *env, const char *name);

/* execute.c 5 */
//int	ft_execute(t_mini *mini);
//int	ft_execute_cmd(t_token *tokens, t_mini *mini);
void			ft_execute_simple_cmd(t_mini *mini, int i);
void			ft_execute_parent(t_mini *mini, int i, int pipe[2], int pid);
void			ft_execute_unit(t_mini *mini, int i);
int				ft_execute(t_mini *mini);

/* execute_pipe.c 3 */
void			ft_execute_last(t_mini *mini, int i);
void			ft_execute_child(t_mini *mini, int i, int	pipe_fd[2]);

/* execute_cmd.c 5    */
void			ft_fill_cmd(t_token **tokens, t_mini *mini);
void			ft_exe_cmd(t_mini *mini, int i);

/* execute_fill_cmd.c 5 */
void			ft_shift_in_out(t_token	**redir, t_token_type type);
void			ft_here_doc(t_token	**heredoc);
void			ft_add_cmd(t_token	**cmd, t_token_type type);

/* execute_utils.c 5*/
char			**ft_env_to_array(t_env *env);
void			ft_free_array(char ***paths);
char			*ft_get_path_from_env(t_env *env);
char			*ft_get_path(char *cmd, t_env *env);

/* execute_built_in.c 4*/
int				ft_cmd_type(char *cmd);
int				ft_is_built_in(char *cmd, t_token *tokens, t_mini *mini);
int				ft_env(t_env *env);
void 			ft_cd_export_unset(t_mini *mini, int i);

/*  export.c */
int				ft_export(t_token *tokens, t_env **env);
int				ft_update_var(t_env *env, char *name, char *value);

/* cd.c */
int				ft_cd(t_token *tokens, t_env *env);
int				ft_pwd(void);

/* echo.c */
int				ft_echo(t_token *tokens, t_env *env);

/* unset */
int				ft_unset(t_token *tokens, t_env **env);

/* ft_error_ctr.c 5*/
int				ft_syntax_err_ctr(t_token *lexer);
void 			ft_file_ctr(int fd, char	*msg);
void			ft_set_g_status(char	*msg, int value);
char			*ft_check_path_validity(t_mini *mini, char *path);

/* ft_test_use.c */
void			ft_print_token(t_token	*t);    // ** test function
void			ft_print_cmdarray(char	**cmds);// ** test function
void			ft_test_log(t_mini  *mini);     // ** test function

#endif
