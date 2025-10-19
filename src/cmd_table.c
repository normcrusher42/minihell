/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42adbudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 17:24:16 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/19 18:59:04 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

// Parses a single token, updating the command struct accordingly.
static int	parse_segment_token(t_cmd *cur, t_token *tk, int i, t_parse_ctx *p)
{
	int		k;

	k = redir_kind(tk->tokens[i]);
	if (k >= 0)
	{
		if (!tk->tokens[i + 1] || is_pipe(tk->tokens[i + 1])
			|| redir_kind(tk->tokens[i + 1]) >= 0)
			return (syntax_err(tk->tokens[i + 1], p->st, NULL));
		if (k == R_HEREDOC)
			p->is_quoted = tk->quoted[i + 1];
		if (!push_redir(cur, k, tk->tokens[i + 1]))
			return (0);
		return (2);
	}
	if (!push_word(cur, tk->tokens[i]))
		return (0);
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

// The main function to parse the command table from tokens.
int	parse_command_table(t_shell *sh, int *st)
{
	t_parse_ctx	p;

	p.tk = sh->token;
	p.st = st;
	p.is_quoted = NO;
	if (!init_parse_ctx(&p))
		return (0);
	while (p.tk->tokens[++p.i])
		if (!handle_token(&p))
			return (0);
	if (!finalize_segment(&p.arr, &p.n, &p.cur))
	{
		if (p.n == 0)
			return (handle_empty_parse(sh, &p));
		free_cmd_table(sh);
		free_one_cmd(&p.cur);
		return (syntax_err(NULL, p.st, NULL));
	}
	sh->cmds = p.arr;
	sh->ncmd = p.n;
	sh->is_quoted = p.is_quoted;
	return (1);
}
