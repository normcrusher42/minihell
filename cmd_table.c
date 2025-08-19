/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:44:42 by lsahloul          #+#    #+#             */
/*   Updated: 2025/08/16 15:48:12 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_cmd(t_cmd *c)
{
	c->argv = NULL;
	c->argc = 0;
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

/* ------------------------ dyn arrays (argv/redirs) ------------------------ */

static int	push_word(t_cmd *c, const char *w)
{
	char	**nv;
	int		i;

	nv = (char **)malloc(sizeof(char *) * (c->argc + 2));
	if (!nv)
		return (0);
	i = -1;
	while (++i < c->argc)
		nv[i] = c->argv[i];
	nv[i] = ft_strdup(w);
	if (!nv[i])
		return (free(nv), 0);
	nv[i + 1] = NULL;
	free(c->argv);
	c->argv = nv;
	c->argc++;
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

/* ------------------------ frees & errors ---------------------------------- */

static void	free_one_cmd(t_cmd *c)
{
	int	i;

	if (!c)
		return ;
	i = -1;
	while (++i < c->argc)
		free(c->argv[i]);
	free(c->argv);
	i = -1;
	while (++i < c->redir_count)
		free(c->redirs[i].arg);
	free(c->redirs);
	init_cmd(c);
}

static int	syntax_err(const char *tok, int *st)
{
	ft_putstr_fd("minishell: syntax error near `", 2);
	ft_putstr_fd((char *)(tok ? tok : "newline"), 2);
	ft_putstr_fd("'\n", 2);
	if (st)
		*st = 258;
	return (0);
}


static int	finalize_segment(t_cmd **arr, int *n, t_cmd *cur)
{
	t_cmd	*nv;
	int		i;

	if (cur->argc == 0 && cur->redir_count == 0)
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

	k = redir_kind(tk->tokens[i]);
	if (k >= 0)
	{
		if (!tk->tokens[i + 1] || is_pipe(tk->tokens[i + 1])
			|| redir_kind(tk->tokens[i + 1]) >= 0)
			return (syntax_err(tk->tokens[i + 1], st));
		if (!push_redir(cur, k, tk->tokens[i + 1],
				(k == R_HEREDOC && tk->quote) ? (tk->quote[i + 1] != QTE_NONE)
				: 0))
			return (0);
		return (2);
	}
	if (!push_word(cur, tk->tokens[i]))
		return (0);
	return (1);
}

int	parse_command_table(t_token *tk, t_cmd **out, int *count, int *st)
{
	t_cmd	cur;
	t_cmd	*arr;
	int		n;
	int		i;
	int		step;

	if (!tk || !tk->tokens || !out || !count)
		return (0);
	arr = NULL;
	n = 0;
	i = -1;
	init_cmd(&cur);
	if (tk->tokens[0] && is_pipe(tk->tokens[0]))
		return (syntax_err(tk->tokens[0], st));
	while (tk->tokens[++i])
	{
		if (is_pipe(tk->tokens[i]))
		{
			if (!finalize_segment(&arr, &n, &cur))
				return (syntax_err(tk->tokens[i], st));
			continue ;
		}
		step = parse_segment_token(&cur, tk, i, st);
		if (step == 0)
			return (free_one_cmd(&cur), free_cmd_table(arr, n), 0);
		i += (step - 1);
	}
	if (!finalize_segment(&arr, &n, &cur))
		return (free_one_cmd(&cur), syntax_err(NULL, st));
	*out = arr;
	*count = n;
	return (1);
}


void	free_cmd_table(t_cmd *cmds, int n)
{
	int	i;
	int	j;

	if (!cmds)
		return ;
	i = -1;
	while (++i < n)
	{
		j = -1;
		while (++j < cmds[i].argc)
			free(cmds[i].argv[j]);
		free(cmds[i].argv);
		j = -1;
		while (++j < cmds[i].redir_count)
			free(cmds[i].redirs[j].arg);
		free(cmds[i].redirs);
	}
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
		while (++j < cmds[i].argc)
			ft_printf("  argv[%d]=`%s`\n", j, cmds[i].argv[j]);
		j = -1;
		while (++j < cmds[i].redir_count)
			ft_printf("  redir[%d]=%d `%s` q=%d\n", j,
				cmds[i].redirs[j].type,
				cmds[i].redirs[j].arg,
				cmds[i].redirs[j].is_quoted);
	}
}

