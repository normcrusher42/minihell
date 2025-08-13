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