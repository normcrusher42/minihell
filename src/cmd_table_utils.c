/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 18:52:40 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/22 20:52:58 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The entire following was done by @Leen */
//	   init_cmd
//	   is_pipe
//	   redir_kind
//	   init_parse_ctx
//	   handle_empty_parse

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
int	is_pipe(const char *s)
{
	return (s && s[0] == '|' && s[1] == '\0');
}

// Returns the redirection type based on the token string.
int	redir_kind(const char *s)
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
int	init_parse_ctx(t_parse_ctx *p)
{
	p->arr = NULL;
	p->n = 0;
	p->i = -1;
	init_cmd(&p->cur);
	if (!p->tk || !p->tk->tokens)
		return (0);
	if (p->tk->tokens[0] && is_pipe(p->tk->tokens[0])
		&& !(p->tk->quoted && p->tk->quoted[0]))
		return (syntax_err(p->tk->tokens[0], p->st, NULL));
	return (1);
}

// Handles the case of an empty parse (no commands).
int	handle_empty_parse(t_shell *sh, t_parse_ctx *p)
{
	sh->cmds = NULL;
	sh->ncmd = 0;
	sh->is_quoted = p->is_quoted;
	sh->ex_st = 0;
	return (1);
}
