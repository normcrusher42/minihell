#include "minishell.h"

void	execute_command(char *cmd, char **env)
{
	pid_t	pid;
	char	*av[2];
	int		status;

	pid = fork();
	av[0] = cmd;
	av[1] = NULL;
	status = 0;
	if (pid == 0)
	{
		execve(cmd, av, env);
		perror(RED "and u faillled!!!" RESET);
		exit(127);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	else
		perror(RED "and u faillled!!!" RESET);
}
