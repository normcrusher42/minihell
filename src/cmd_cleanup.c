/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cleanup.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.a    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 02:56:10 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/12 02:56:10 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper for freeing a single command struct.
void	free_one_cmd(t_cmd *c)
{
	int	i;

	if (!c)
		return ;
	i = -1;
	if (c->av)
	{
		while (++i < c->ac)
			if (c->av[i])
				free(c->av[i]);
		free(c->av);
	}
	i = -1;
	if (c->redirs)
	{
		while (++i < c->redir_count)
			if (c->redirs[i].arg)
				free(c->redirs[i].arg);
		free(c->redirs);
	}
	init_cmd(c);
}

// Frees the entire command table in the shell struct.
void	free_cmd_table(t_shell *sh)
{
	int	i;

	if (!sh->cmds)
		return ;
	i = -1;
	while (++i < sh->ncmd)
		free_one_cmd(&sh->cmds[i]);
	free(sh->cmds);
	sh->cmds = NULL;
	sh->ncmd = 0;
}

// Helper for freeing a temporary cmd table
void	free_cmd_table_ctx(t_cmd *cmds, int n)
{
	int	i;

	i = 0;
	if (!cmds)
		return ;
	while (i < n)
		free_one_cmd(&cmds[i++]);
	free(cmds);
}

// Handles syntax errors by printing a message and setting the status.
int	syntax_err(const char *tok, int *st, t_parse_ctx *p)
{
	ft_putstr_fd("miniOdy: syntax error near `", 2);
	if (tok)
		ft_putendl_fd((char *)tok, 2);
	else
		ft_putendl_fd("newline", 2);
	if (p)
		free_cmd_table_ctx(p->arr, p->n);
	if (st)
		*st = 258;
	return (0);
}
