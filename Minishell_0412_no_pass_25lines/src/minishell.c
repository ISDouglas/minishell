/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: layang <layang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 09:44:04 by nimorel           #+#    #+#             */
/*   Updated: 2025/04/11 19:09:24 by layang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_status;

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
	g_status = 130;
}

/* Order : see drive doc
		Bash 遵循「先处理重定向，再执行命令」的顺序
		解析整条命令 -> 处理重定向 ->检查文件->查找命令->执行管道
		(ls | grep srgdg dd): we dont need to indentify dd before pipe */
/* static int	ft_input_ctr(t_token	*tokens)
{
	
	return (0);
} */

void	ft_init_mini(t_mini	*mini, char **envp)
{
	g_status = 0;
	mini->input = NULL;
	mini->lexer = NULL;
	mini->exe_tab = NULL;
	mini->tab_size = 0;
	mini->env = ft_init_env(envp);
	mini->array_env = NULL;
	mini->cmd_array = NULL;
	mini->pre = -1;
	mini->log_fd = -1;                   // **for testing log
	mini->stdout_fd = dup(STDOUT_FILENO);// **for testing log           
}

static void	ft_prompt(t_mini	*mini)
{
	while (1)
	{
		mini->pre = -1;
		dup2(mini->stdout_fd, STDOUT_FILENO);			// ** for testing log
		mini->input = readline(PURPLEB "minishell" X YELLOW "$ " X);
		if (!mini->input)
			break ;
		if (mini->input[0])
		{
			ft_test_log(mini);	                    // **for testing log
			add_history(mini->input);		
			if (ft_lexer(mini) == 2)// part input ctr is in ft_count_unit
			{                     
				ft_free_mini(mini, 0);   // **for testing
				close(mini->log_fd);     // **for testing
				continue ;
			}
			if (mini->tab_size == 1                  
				&& !ft_strncmp(mini->exe_tab[0]->value, "exit", 5))
				break ;		                         // exit command
			if (mini->exe_tab && mini->env)
				g_status = ft_execute(mini);
			ft_free_mini(mini, 0);	                    
		}
	}
}

/* 
export MY_FILE=output.txt
echo "Hello, World!" > $MY_FILE
 */

int	main(int argc, char	**argv, char **envp)
{
	t_mini	mini;
	
	(void)argc;
	(void)argv;
	ft_start_animation();
	signal(SIGINT, ft_handle_sigint); //use ctr+C when heredoc?
	signal(SIGQUIT, SIG_IGN);
	ft_init_mini(&mini, envp);
	ft_prompt(&mini);                    
	ft_free_mini(&mini, 1);
	return (0);
}

/* int	main(int argc, char	**argv, char **envp)
{
	t_mini	mini;
	
	(void)argc;
	(void)argv;
	ft_start_animation();
	signal(SIGINT, ft_handle_sigint); //use ctr+C when heredoc?
	signal(SIGQUIT, SIG_IGN);
	ft_init_mini(&mini, envp);
	while (1)
	{
		mini.pre = -1;
		dup2(mini.stdout_fd, STDOUT_FILENO);			// ** for testing log
		mini.input = readline(PURPLEB "minishell" X YELLOW "$ " X);
		if (!mini.input)
			break ;
		if (mini.input[0])
		{
			ft_test_log(&mini);	                    // **for testing log
			add_history(mini.input);		
			ft_lexer(&mini);
			//ft_input_ctr(mini.lexer) == -1)
			if (g_status == 2)                      // syntax error
				continue ;
			if (mini.tab_size == 1                  
				&& !ft_strncmp(mini.exe_tab[0]->value, "exit", 5))
				break ;		                         // exit command
			if (mini.exe_tab && mini.env)
				g_status = ft_execute(&mini);
			//printf("Cmd finish. sign :%d\n", mini.sign);
			ft_free_mini(&mini, 0);
			close(mini.log_fd);                             // ** for testing log 
			//and rm heredoc_tmp maybe in free_0;
		}
	}
	close(mini.stdout_fd);                               // ** for testing log
	ft_free_mini(&mini, 1);
	clear_history();
	return (0);
} */

/*
Recommand commands (mandatory part):
ls | exit | echo "y" （if "exit" is in the pipe, should pass to next cmd）
grep c << e1 << e2 | ls src
cat << e1 << e2
<> coucou cat
..|<| ..
..|grep | .. (do not forget to close a fd)
cat | grep he
cat
<<


Recommand commands (bonus part):
(echo "1" || cat op) && ls || (cat tt || echo "end"

test:
open a new terminal:
gnome-terminal -- bash -c "grep c << e1 << e2 | ls; exec bash"


echo "Y" || ls | echo "end"
$Y
echo "Y"：这个命令会输出 Y，并且成功执行（返回状态码为0）。
||：这是一个逻辑或运算符。只有当 echo "Y" 失败（返回非0状态码）时，后面的命令 ls 才会执行。
但由于 echo "Y" 成功，因此 ls 不会执行。
|：这个管道符用于将 ls 的输出传递给 echo "end"。但是因为 ls 没有被执行，所以没有输出可以传递。
因此，echo "end" 不会执行，因为 ls 没有执行，而 echo "end" 是与 ls 通过管道连接的。

---Watch out---
basic rule: identify by order
ex: (echo "1" || cat op) && ls | | (cat tt || echo "end"
rule 1: double pipe (should give error on systax and stop, exit code : 2)
rule 2: unfinish parrenthesis (should be executed)


to correct:

$ <<>>Makefile
bash: syntax error near unexpected token `>>'

---varify each file's exit before start!
ex: $ ls | wc -c | < frdfhj grep e
bash: frdfhj: No such file or directory

$ lss | wcll -c | cat frdfhj
cat: frdfhj: No such file or directory
Command 'lss' not found, but there are 15 similar ones.
Command 'wcll' not found, did you mean:
  command 'wall' from deb bsdutils (1:2.37.2-4ubuntu3.4)
Try: apt install <deb name>

$ lss | wcll -c | < frdfhj grep e
bash: frdfhj: No such file or directory
Command 'wcll' not found, did you mean:
  command 'wall' from deb bsdutils (1:2.37.2-4ubuntu3.4)
Try: apt install <deb name>
Command 'lss' not found, but there are 15 similar ones.

Order : see drive doc
		Bash 遵循「先处理重定向，再执行命令」的顺序
		解析整条命令 -> 处理重定向 ->检查文件->查找命令->执行管道
		(ls | grep srgdg dd): we dont need to indentify dd before pipe
*/
//grep he out | cat < out2
//grep he out > out2 | cat


/* in real bash, open all pipe at the same time, 
and collect STDIN and STDOUT all together
so in the cmd below only first pipe be executed.

$ echo "ed1" "hel2" >> out | grep he < out >> out2 | grep he < out2 >> out
bash: out: No such file or directory
bash: out2: No such file or directory
$ ls
out
$ cat out
ed1 hel2

 */
//                   wrong cmd
//echo ed2 hel2 | grep he >> out2 | grep he >> out
//               open heredoc to grep

//                        good cmd
//echo ed2 hel2 > out | grep he >> out2 out| grep he out
//                        good cmd 
//echo ed4 hel4 | grep he >>out | grep he < out >> out3

//              good cmd but "grep he >> out3" will not execute
//echo ed4 hel4 | grep he >>out | grep he >> out3

// echo "hello from 3 command4" >> output2.txt | grep "3" < output.txt > output3.txt


//echo "35" >> output2.txt | grep 3 > output3.txt

