/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:57:13 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/10 15:57:13 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* The entire following was done by @Nasser */
//	   handle_unexpected_eof
//	   count_without_quotes
//	   remove_quotes
//	   process_all_tokens

#include "minishell.h"

// Handles EoF error output with exit code check.
int	handle_unexpected_eof(t_shell *sh)
{
	ft_putendl_fd(UNEXPECTED_EOF, 2);
	ft_putendl_fd(SYNTAX_ERROR, 2);
	if (sh)
	{
		sh->ex_st = 258;
		sh->err = YES;
	}
	return (-1);
}

// Counts the length of the string without quotes and checks for errors.
static int	count_without_quotes(const char *s, t_shell *sh)
{
	int	i;
	int	len;
	int	in_quote;

	i = -1;
	len = 0;
	in_quote = 0;
	while (s[++i])
	{
		if (!in_quote && (s[i] == '\'' || s[i] == '"'))
			in_quote = s[i];
		else if (in_quote && s[i] == in_quote)
			in_quote = 0;
		else
			len++;
	}
	if (in_quote)
		return (handle_unexpected_eof(sh));
	return (len);
}

// Removes quotes from a given string and returns a new allocated string.
// Returns NULL on allocation failure or syntax error.
char	*remove_quotes(const char *s, t_shell *sh)
{
	t_quote_vars	qv;
	int				len;

	qv = (t_quote_vars){0};
	if (!s)
		return (NULL);
	len = count_without_quotes(s, sh);
	if (len < 0)
		return (NULL);
	qv.res = malloc(len + 1);
	if (!qv.res)
		return (NULL);
	while (s[qv.i])
	{
		if (!qv.in_quote && (s[qv.i] == '\'' || s[qv.i] == '"'))
			qv.in_quote = s[qv.i];
		else if (qv.in_quote && s[qv.i] == qv.in_quote)
			qv.in_quote = 0;
		else
			qv.res[qv.j++] = s[qv.i];
		qv.i++;
	}
	qv.res[qv.j] = '\0';
	return (qv.res);
}

// Processes all tokens in the shell struct: 
// expands variables and removes quotes.
void	process_all_tokens(t_shell *sh, char **envp)
{
	char	**expanded_tokens;
	int		i;
	char	*cleaned;

	if (!sh || !sh->token || !sh->token->tokens)
		return ;
	expanded_tokens = expand_token(sh, envp, sh->token);
	if (!expanded_tokens)
		return ;
	free(sh->token->tokens);
	sh->token->tokens = expanded_tokens;
	i = 0;
	while (sh->token->tokens[i])
	{
		cleaned = remove_quotes(sh->token->tokens[i], sh);
		if (!cleaned)
			return ;
		free(sh->token->tokens[i]);
		sh->token->tokens[i] = cleaned;
		i++;
	}
}
