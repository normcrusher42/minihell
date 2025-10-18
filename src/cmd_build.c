/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_build.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:40:49 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/18 18:49:06 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Adds a word to the command's argument list.
static int	push_word(t_cmd *c, const char *w)
{
	char	**nv;
	int		i;

	nv = (char **)malloc(sizeof(char *) * (c->ac + 2));
	if (!nv)
		return (0);
	i = -1;
	while (++i < c->ac)
		nv[i] = c->av[i];
	nv[i] = ft_strdup(w);
	if (!nv[i])
		return (free(nv), 0);
	nv[i + 1] = NULL;
	free(c->av);
	c->av = nv;
	c->ac++;
	return (1);
}

// Adds a redirection to the command's redirection list.
static int	push_redir(t_cmd *c, t_redirtype t, const char *arg)
{
	t_redir	*nr;
	int		i;

	nr = (t_redir *)malloc(sizeof(t_redir) * (c->redir_count + 1));
	if (!nr)
		return (0);
	i = -1;
	while (++i < c->redir_count)
		nr[i] = c->redirs[i];
	nr[i].type = t;
	nr[i].arg = ft_strdup(arg);
	if (!nr[i].arg)
		return (free(nr), 0);
	free(c->redirs);
	c->redirs = nr;
	c->redir_count++;
	return (1);
}

// Finalizes the current command segment and adds it to the command array.
static int	finalize_segment(t_cmd **arr, int *n, t_cmd *cur)
{
	t_cmd	*nv;
	int		i;

	if (cur->ac == 0 && cur->redir_count == 0)
		return (0);
	nv = (t_cmd *)malloc(sizeof(t_cmd) * (*n + 1));
	if (!nv)
		return (0);
	i = -1;
	while (++i < *n)
		nv[i] = (*arr)[i];
	init_cmd(&nv[i]);
	if (!nv)
		return (0);
	i = -1;
	while (++i < *n)
		nv[i] = (*arr)[i];
	nv[i] = *cur;
	free(*arr);
	*arr = nv;
	(*n)++;
	init_cmd(cur);
	return (1);
}

// Handles a single token during parsing, updating the context.
static int	handle_token(t_parse_ctx *p)
{
	int	step;

	if (is_pipe(p->tk->tokens[p->i]))
	{
		if (!finalize_segment(&p->arr, &p->n, &p->cur))
			return (syntax_err(p->tk->tokens[p->i], p->st, p));
	}
	else
	{
		step = parse_segment_token(&p->cur, p->tk, p->i, p);
		if (step == 0)
		{
			free_one_cmd(&p->cur);
			free_cmd_table_ctx(p->arr, p->n);
			return (0);
		}
		p->i += (step - 1);
	}
	return (1);
}
