
#include "minishell.h"

void	call_janitor(t_shell *sh)
{
	if (!sh)
		return ;
	free_arr(&shell->envp, NO);
	free(sh->line);
}
