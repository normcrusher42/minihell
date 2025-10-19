/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:18:49 by lsahloul          #+#    #+#             */
/*   Updated: 2025/09/29 20:18:49 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Expands the token array to accommodate a new token.
static void	expand_token_arrays(t_token *tok, int count)
{
	char	**new_tokens;
	int		*new_quoted;
	int		i;

	new_tokens = malloc(sizeof(char *) * (count + 2));
	new_quoted = malloc(sizeof(int) * (count + 2));
	if (!new_tokens || !new_quoted)
		return ;
	i = -1;
	while (++i < count)
	{
		new_tokens[i] = tok->tokens[i];
		new_quoted[i] = tok->quoted[i];
	}
	new_tokens[count] = NULL;
	new_tokens[count + 1] = NULL;
	new_quoted[count] = 0;
	new_quoted[count + 1] = 0;
	free(tok->tokens);
	free(tok->quoted);
	tok->tokens = new_tokens;
	tok->quoted = new_quoted;
}

// Stores a new token in the token struct.
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
	tok->quoted[count] = is_quoted_token(value);
	tok->tokens[count + 1] = NULL;
	tok->quoted[count + 1] = 0;
}

// Skips spaces and operators, storing operators as tokens.
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

// Reads a word token, handling quoted sections properly.
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

// Tokenizes the input string and stores tokens in the shell struct.
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
	sh->token->quoted = NULL;
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
