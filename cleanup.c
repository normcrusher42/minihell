
#include "minishell.h"

void	call_janitor(t_shell *sh)
{
	if (!sh)
		return ;
	free_arr(&sh->envp, NO);
	free(sh->input);
}
