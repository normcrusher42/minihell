#include "minishell.h"

void	add_pwd_var(char **envp)
{
	char	*pwd;

	pwd = get_env_value(envp, "PWD");
	if (pwd)
		if (getcwd(pwd, sizeof(char)))
			set_env_value(&envp, "PWD", pwd);	
}