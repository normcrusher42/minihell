/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:44:42 by lsahloul          #+#    #+#             */
/*   Updated: 2025/09/04 21:29:29 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_cmd(t_cmd *c)
{
	c->av = NULL;
	c->ac = 0;
	c->redirs = NULL;
	c->redir_count = 0;
}

static int	is_pipe(const char *s)
{
	return (s && s[0] == '|' && s[1] == '\0');
}

static int	redir_kind(const char *s)
{
	if (!s)
		return (-1);
	if (s[0] == '<' && s[1] == '\0')
		return (R_IN);
	if (s[0] == '>' && s[1] == '\0')
		return (R_OUT);
	if (s[0] == '>' && s[1] == '>' && s[2] == '\0')
		return (R_APP);
	if (s[0] == '<' && s[1] == '>' && s[2] == '\0')
		return (-1);
	if (s[0] == '<' && s[1] == '<' && s[2] == '\0')
		return (R_HEREDOC);
	return (-1);
}

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

static int	push_redir(t_cmd *c, t_redirtype t, const char *arg, int quoted)
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
	nr[i].is_quoted = quoted;
	if (!nr[i].arg)
		return (free(nr), 0);
	free(c->redirs);
	c->redirs = nr;
	c->redir_count++;
	return (1);
}

static void	free_one_cmd(t_cmd *c)
{
	int	i;

	if (!c)
		return ;
	i = -1;
	while (++i < c->ac)
		free(c->av[i]);
	free(c->av);
	i = -1;
	while (++i < c->redir_count)
		free(c->redirs[i].arg);
	free(c->redirs);
	init_cmd(c);
}

static int	syntax_err(const char *tok, int *st)
{
	ft_putstr_fd("minishell: syntax error near `", 2);
	if (tok)
		ft_putstr_fd((char *)tok, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	if (st)
		*st = 258;
	return (0);
}

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
	nv[i] = *cur;
	free(*arr);
	*arr = nv;
	(*n)++;
	init_cmd(cur);
	return (1);
}

static int	parse_segment_token(t_cmd *cur, t_token *tk, int i, int *st)
{
	int	k;
	int	is_quoted;

	k = redir_kind(tk->tokens[i]);
	if (k >= 0)
	{
		if (!tk->tokens[i + 1] || is_pipe(tk->tokens[i + 1])
			|| redir_kind(tk->tokens[i + 1]) >= 0)
			return (syntax_err(tk->tokens[i + 1], st));
		is_quoted = 0;
		if (k == R_HEREDOC && tk->quote)
		{
			if (tk->quote[i + 1] != QTE_NONE)
				is_quoted = 1;
		}
		if (!push_redir(cur, k, tk->tokens[i + 1], is_quoted))
			return (0);
		return (2);
	}
	if (!push_word(cur, tk->tokens[i]))
		return (0);
	return (1);
}

static int	init_parse_ctx(t_parse_ctx *p)
{
	p->arr = NULL;
	p->n = 0;
	p->i = -1;
	init_cmd(&p->cur);
	if (!p->tk || !p->tk->tokens)
		return (0);
	if (p->tk->tokens[0] && is_pipe(p->tk->tokens[0]))
		return (syntax_err(p->tk->tokens[0], p->st));
	return (1);
}

static int	handle_token(t_parse_ctx *p)
{
	int	step;

	if (is_pipe(p->tk->tokens[p->i]))
	{
		if (!finalize_segment(&p->arr, &p->n, &p->cur))
			return (syntax_err(p->tk->tokens[p->i], p->st));
	}
	else
	{
		step = parse_segment_token(&p->cur, p->tk, p->i, p->st);
		if (step == 0)
		{
			free_one_cmd(&p->cur);
			free_cmd_table(p->arr, p->n);
			return (0);
		}
		p->i += (step - 1);
	}
	return (1);
}

int	parse_command_table(t_token *tk, t_cmd **out, int *count, int *st)
{
	t_parse_ctx	p;

	p.tk = tk;
	p.st = st;
	if (!out || !count || !init_parse_ctx(&p))
		return (0);
	while (p.tk->tokens[++p.i])
		if (!handle_token(&p))
			return (0);
	if (!finalize_segment(&p.arr, &p.n, &p.cur))
	{
		free_one_cmd(&p.cur);
		return (syntax_err(NULL, p.st));
	}
	*out = p.arr;
	*count = p.n;
	return (1);
}

void	free_cmd_table(t_cmd *cmds, int n)
{
	int	i;

	if (!cmds)
		return ;
	i = -1;
	while (++i < n)
		free_one_cmd(&cmds[i]);
	free(cmds);
}

void	print_cmd_table(t_cmd *cmds, int n)
{
	int	i;
	int	j;

	if (!cmds)
		return ;
	i = -1;
	while (++i < n)
	{
		ft_printf("cmd[%d]:\n", i);
		j = -1;
		while (++j < cmds[i].ac)
			ft_printf("  av[%d]=`%s`\n", j, cmds[i].av[j]);
		j = -1;
		while (++j < cmds[i].redir_count)
			ft_printf("  redir[%d]=%d `%s` q=%d\n", j,
				cmds[i].redirs[j].type,
				cmds[i].redirs[j].arg,
				cmds[i].redirs[j].is_quoted);
	}
}
