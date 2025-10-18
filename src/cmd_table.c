/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 17:24:16 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/18 18:49:56 by lsahloul         ###   ########.fr       */
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

static int	handle_empty_segment(t_parse_ctx *p, t_shell *sh)
{
	sh->cmds = NULL;
	sh->ncmd = 0;
	sh->is_quoted = p->is_quoted;
	return (1);
}
