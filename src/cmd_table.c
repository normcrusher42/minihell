/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid Date        by              +#+  #+#    #+#             */
/*   Updated: 2025/10/18 17:24:04 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "minishell.h"

// Initializes a command struct to default values.
void	init_cmd(t_cmd *c)
{
	c->av = NULL;
	c->ac = 0;
	c->redirs = NULL;
	c->redir_count = 0;
}

// Returns 1 if the token is a pipe operator.
static int	is_pipe(const char *s)
{
	return (s && s[0] == '|' && s[1] == '\0');
}

// Returns the redirection type based on the token string.
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
	init_cmd(&nv[i]);  // Initialize the new command
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

// Initializes the parsing context and checks for initial syntax errors.
static int	init_parse_ctx(t_parse_ctx *p)
{
	p->arr = NULL;
	p->n = 0;
	p->i = -1;
	init_cmd(&p->cur);
	if (!p->tk || !p->tk->tokens)
		return (0);
	if (p->tk->tokens[0] && is_pipe(p->tk->tokens[0]))
		return (syntax_err(p->tk->tokens[0], p->st, NULL));
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
		/* If no segments were produced and current command is empty,
		   treat this as no-op (e.g. all tokens expanded to empty) and
		   return success with no commands. */
		if (p.n == 0)
		{
			sh->cmds = NULL;
			sh->ncmd = 0;
			sh->is_quoted = p.is_quoted;
			return (1);
		}
		free_cmd_table(sh);
		free_one_cmd(&p.cur);
		return (syntax_err(NULL, p.st, NULL));
	}
	sh->cmds = p.arr;
	sh->ncmd = p.n;
	sh->is_quoted = p.is_quoted;
	return (1);
}

/*							remove this later if done						  */
void	print_cmd_table(t_shell *sh)
{
	int	i;
	int	j;

	if (!sh->cmds)
		return ;
	i = -1;
	while (++i < sh->ncmd)
	{
		ft_printf("cmd[%d]:\n", i);
		j = -1;
		while (++j < sh->cmds[i].ac)
			ft_printf("  av[%d]=`%s`\n", j, sh->cmds[i].av[j]);
		j = -1;
		while (++j < sh->cmds[i].redir_count)
			ft_printf("  redir[%d]=%d `%s` q=%d\n", j,
				sh->cmds[i].redirs[j].type,
				sh->cmds[i].redirs[j].arg,
				sh->cmds[i].redirs[j].is_quoted);
	}
}
