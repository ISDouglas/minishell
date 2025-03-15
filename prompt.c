#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

void print_env(char **env)
{
    int i = 0;

    while (env[i])
    {
        printf("%s\n", env[i]);
        i++;
    }
}

int main(int argc, char **argv, char **envp)
{
    char *input;

    (void)argc;
    (void)argv;
	while (1)
    {
        input = readline("minishell$ ");
        if (!input)
        {
            write(1, "exit\n", 5);
            break;
        }
		if (input[0] && strcmp(input, "env") == 0)
        {
            print_env(envp);
        }
        //add_history(input); // Save command to history
        free(input);
    }
    return (0);
}


/* int main(void)
{
    char *rl;
	while (1)
	{
		rl = readline("Prompt > ");
		//printf("%s\n", rl);
	}
    return (0);
} */
//cc prompt.c -o minishell -lreadline
// -lreadline