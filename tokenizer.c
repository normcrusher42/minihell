/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 14:47:32 by lsahloul          #+#    #+#             */
/*   Updated: 2025/08/10 19:24:15 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char			**new_tokens;
	t_quote_type	*new_quotes;
	int				i;

	new_tokens = malloc(sizeof(char *) * (count + 2));
	new_quotes = malloc(sizeof(t_quote_type) * (count + 2));
	if (!new_tokens || !new_quotes)
	{
		free(new_tokens);
		free(new_quotes);
		return ;
	}
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
{
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

static void	skip_spaces_operators(char *s, int *i, t_token *tok)
{
	int	len;

	if (s[*i] == ' ' || s[*i] == '\t')
	{
		(*i)++;
		return ;
	}
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

t_token	*tokenize(char *s)
{
	t_token			*tok;
	int				i;
	int				start;
	t_quote_type	qt;

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

	i = 0;
	if (!tok)
		return ;
	while (tok->tokens && tok->tokens[i])
	{
		free(tok->tokens[i]);
		i++;
	}
	free(tok->tokens);
	free(tok->quote);
	free(tok);
}
