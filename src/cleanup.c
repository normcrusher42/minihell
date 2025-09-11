/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 19:37:31 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 19:37:31 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Frees the passed double array pointer and its content (with a reuse switch). 
void	free_arr(char ***arr, bool reuse)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = -1;
	while ((*arr)[++i])
	{
		free((*arr)[i]);
		(*arr)[i] = NULL;
	}
	if (!reuse)
	{
		free(*arr);
		*arr = NULL;
	}
}

// Frees the stored input.
void	butter_free_input(t_shell *shell)
{
	if (shell->input)
	{
		free(shell->input);
		shell->input = NULL;
	}
}

// The all-in-one cleanup function for the exit function.
void	call_janitor(t_shell *sh)
{
	if (!sh)
		return ;
	free_arr(&sh->envp, NO);
	free_arr(&sh->token.tokens, NO);
	free(sh->input);
}
