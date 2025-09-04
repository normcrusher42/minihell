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

void	call_janitor(t_shell *sh)
{
	if (!sh)
		return ;
	free_arr(&sh->envp, NO);
	free(sh->input);
}
