#include "minishell.h"

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	skip_quotes(char *s, char quote)
{
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
	char	**new_tokens;
	int		i;

	new_tokens = malloc(sizeof(char *) * (count + 2));
	if (!new_tokens)
		return ;
	i = -1;
	while (++i < count)
		new_tokens[i] = tok->tokens[i];
	new_tokens[count] = NULL;
	new_tokens[count + 1] = NULL;
	free(tok->tokens);
	tok->tokens = new_tokens;
}

static void	store_token_struct(t_token *tok, char *value)
{
	int	count;

	count = 0;
	while (tok->tokens && tok->tokens[count])
		count++;
	expand_token_arrays(tok, count);
	if (!tok->tokens)
		return ;
	tok->tokens[count] = value;
	tok->tokens[count + 1] = NULL;
}

static void	consume_spaces(char *s, int *i)
{
	while (s[*i] == ' ' || s[*i] == '\t')
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
		store_token_struct(tok, ft_substr(s, *i, len));
		*i += len;
	}
}

static void	read_word(char *s, int *i)
{
	while (s[*i] && !is_operator(s[*i]) && s[*i] != ' ' && s[*i] != '\t')
	{
		if (s[*i] == '\'' || s[*i] == '"')
			*i += skip_quotes(&s[*i], s[*i]);
		else
			(*i)++;
	}
}

void	tokenize(char *s, t_shell *sh)
{
	int		i;
	int		start;

	if (!s || !sh)
		return ;
	if (sh->token)
		free_tokens(sh);
	sh->token = malloc(sizeof(t_token));
	if (!sh->token)
		return ;
	sh->token->tokens = NULL;
	i = 0;
	while (s[i])
	{
		skip_spaces_operators(s, &i, sh->token);
		if (!s[i] || is_operator(s[i]) || s[i] == ' ' || s[i] == '\t')
			continue ;
		start = i;
		read_word(s, &i);
		store_token_struct(sh->token, ft_substr(s, start, i - start));
	}
}
