#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_shell
{
	char *input;
} t_shell;

int		main(int argc, char **argv, char **envp);
void	butter_free(t_shell *shell);

#endif