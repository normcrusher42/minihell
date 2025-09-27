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

// Frees the array of tokens from sh, ready for the next call.
void	free_tokens(t_shell *sh)
{
	if (!sh || !sh->token)
		return ;
	if (sh->token->tokens)
		free_arr(&sh->token->tokens, NO);
	if (sh->token)
	{
		free(sh->token);
		sh->token = NULL;
	}
}

// The all-in-one cleanup function for a clean exit.
void	call_janitor(t_shell *sh)
{
	if (!sh)
		return ;
	free_arr(&sh->envp, NO);
	if (sh->input)
		free(sh->input);
	free_cmd_table(sh);
	free_tokens(sh);
}
