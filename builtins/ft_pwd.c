#include "minishell.h"

// A simple remake of 'pwd', prints the current working directory
int	ft_pwd(void)
{
	char	buf[1024];

	if (getcwd(buf, sizeof(buf)))
		return (ft_putendl_fd(buf, 1), 0);
	perror("pwd");
	return (1);
}
