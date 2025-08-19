void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1); //Force a newline 
	rl_replace_line("", 0); //Clear the Readline buffer (erase the user’s partially typed command)
	rl_on_new_line(); //Tell Readline “we’re logically on a new line now”.
	rl_redisplay(); //Repaint the prompt and the (now empty) line
}

void	handle_sigquit(int sig)
{ //By setting an empty handler, you override the default 
    // (which kills the process or prints Quit: 3) and effectively ignore it
	(void)sig;
}

void	init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask); //While running handle_sigint, do not block any additional signals
	sa_int.sa_flags = SA_RESTART; //keeps loop stable
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

////////////////////////////
/* 
turns a raw input string into a sequence of tokens and
tracks which tokens were inside single quotes vs double quotes vs unquoted
Later, the expander uses this to decide where to expand $VAR (not inside single quotes)

*/
#include "minishell.h"

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	skip_quotes(char *s, char quote)
{
    /*
    Today, your tokenizer doesn’t error on unclosed quotes; 
    it just treats the rest as one token.
    Later, you’ll likely want to detect this and throw a syntax error.
    */
	int	i;

	i = 1;
	while (s[i] && s[i] != quote)
		i++;
	if (s[i] == quote)
		i++;
	return (i);
}

static void	expand_token_arrays(t_token *tok, int count)
{
    /*
    Each time you add a token, you:

        1. allocate a bigger array,

        2. copy old pointers,

        3. append a new slot,

        4. keep arrays NULL‑terminated.
    */
	char			**new_tokens;
	t_quote_type	*new_quotes;
	int				i;

	new_tokens = malloc(sizeof(char *) * (count + 2)); //count +2 is one for the new token you’re about to add, and other for null terminator
	new_quotes = malloc(sizeof(t_quote_type) * (count + 2));
	if (!new_tokens || !new_quotes) //come back
		return (free(new_tokens), free(new_quotes), (void)0);
	i = -1;
	while (++i < count)
	{
		new_tokens[i] = tok->tokens[i];
		new_quotes[i] = tok->quote[i];
	}
	new_tokens[count] = NULL;
	new_quotes[count] = QTE_NONE;
	free(tok->tokens);
	free(tok->quote);
	tok->tokens = new_tokens;
	tok->quote = new_quotes;
}

static void	store_token_struct(t_token *tok, char *value, t_quote_type qt)
{ //append one token (value) and its quote type (qt) to the struct
	int	count;

	count = 0;
	while (tok->tokens && tok->tokens[count])
		count++;
	expand_token_arrays(tok, count);
	if (!tok->tokens || !tok->quote)
		return ;
	tok->tokens[count] = value;
	tok->quote[count] = qt;
	tok->tokens[count + 1] = NULL;
	tok->quote[count + 1] = QTE_NONE;
}

static void	consume_spaces(char *s, int *i)
{
    //Eat runs of spaces/tabs in one call.
	while (s[*i] == ' ' || s[*i] == '\t') //confusion
		(*i)++;
}

static void	skip_spaces_operators(char *s, int *i, t_token *tok)
{
	int	len;

	if (s[*i] == ' ' || s[*i] == '\t')
		return (consume_spaces(s, i));
	if (is_operator(s[*i]))
	{
		len = 1;
		if (s[*i] == s[*i + 1] && (s[*i] == '<' || s[*i] == '>'))
			len = 2;
		store_token_struct(tok, ft_substr(s, *i, len), QTE_NONE);
		*i += len;
	}
}

static void	read_word(char *s, int *i, t_quote_type *qt)
{
    /*
    move *i from the start of a word token to the first char after it, 
    while remembering which quotes appeared.

    Stop scanning when you hit: end, operator, or whitespace.

    If you see a ' or ", you:

        -set *qt to QTE_SINGLE or QTE_DOUBLE, and

        -skip the entire quoted chunk (including closing quote if present).
    
    you only need to know 
    “was this token protected by single quotes at all?” for expansion rules.

    If you want perfect bash‑like behavior for tokens with mixed quotes (e.g., he"ll'o"),
    you’d track quote segments, not a single flag. 
    For Week 2, the single flag is plenty.
    */
	*qt = QTE_NONE;
	while (s[*i] && !is_operator(s[*i]) && s[*i] != ' ' && s[*i] != '\t')
	{
		if (s[*i] == '\'')
		{
			*qt = QTE_SINGLE;
			*i += skip_quotes(&s[*i], s[*i]);
		}
		else if (s[*i] == '"')
		{
			*qt = QTE_DOUBLE;
			*i += skip_quotes(&s[*i], s[*i]);
		}
		else
			(*i)++;
	}
}
/*
is this needed?:
Input: weird'mix"ed'
You’ll end at the right index. *qt will be whichever quote you saw last (QTE_DOUBLE here), but the whole token is preserved as a single piece (quotes included). 
Expansion will still be blocked if you decide “any single quotes present” → no expand; your current code checks simply qt != QTE_SINGLE when deciding to expand. 
If you want stricter behavior, you can store a bitmask (saw_single, saw_double).
*/

t_token	*tokenize(char *s)
{
	t_token			*tok;
	int				i;
	int				start;
	t_quote_type	qt;

	if (!s)
		return (NULL);
	i = 0;
	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->tokens = NULL;
	tok->quote = NULL;
	while (s[i])
	{
		skip_spaces_operators(s, &i, tok);
		if (!s[i] || is_operator(s[i]) || s[i] == ' ' || s[i] == '\t')
			continue ;
		start = i;
		read_word(s, &i, &qt);
		store_token_struct(tok, ft_substr(s, start, i - start), qt);
	}
	return (tok);
}

void	free_tokens(t_token *tok)
{
	int	i;

	if (!tok)
		return ;
	i = 0;
	while (tok->tokens && tok->tokens[i])
		free(tok->tokens[i++]);
	free(tok->tokens);
	free(tok->quote);
	free(tok);
}

/////////

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: team                                          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:10:01 by team              #+#    #+#             */
/*   Updated: 2025/08/13 18:10:01 by team             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_TABLE_H
# define CMD_TABLE_H

# include "minishell.h"

/* Redirection types */
/*what kind of redirection:
input <, output >, append >>, heredoc <<*/
typedef enum e_redirtype
{
	R_IN,
	R_OUT,
	R_APP,
	R_HEREDOC
}	t_redirtype;

/* One redirection entry */
typedef struct s_redir
{
	t_redirtype	type;
	char		*arg;       /* file or heredoc delimiter */
	int			is_quoted;  /* heredoc delim quoted? 1/0  */
}	t_redir;

/* Command built by the parser */
typedef struct s_cmd
{
	char	**argv;
	int		argc;
	t_redir	*redirs;
	int		redir_count;
}	t_cmd;

/* Public API */
int		parse_command_table(t_token *tk, t_cmd **out, int *count, int *st);
void	free_cmd_table(t_cmd *cmds, int n);
void	print_cmd_table(t_cmd *cmds, int n); /* only if PARSE_DEBUG defined */

#endif
//////

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_table.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: team                                          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/13 18:10:15 by team              #+#    #+#             */
/*   Updated: 2025/08/13 18:10:15 by team             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd_table.h"

/* ------------------------ tiny helpers ------------------------------------ */

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

/* ------------------------ segment finalize -------------------------------- */

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

/* ------------------------ per-token parse --------------------------------- */

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

/* ------------------------ main entry -------------------------------------- */

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

/* ------------------------ free + (optional) debug ------------------------- */

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

#ifndef PARSE_DEBUG
	(void)cmds;
	(void)n;
#else
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
				cmds[i].redirs[j].type, cmds[i].redirs[j].arg,
				cmds[i].redirs[j].is_quoted);
	}
#endif
}
