/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:06:23 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 22:06:23 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_without_quotes(const char *s, int *err)
{
	int	i;
	int	len;
	int	in_quote;

	i = 0;
	len = 0;
	in_quote = 0;
	while (s[i])
	{
		if (!in_quote && (s[i] == '\'' || s[i] == '"'))
			in_quote = s[i];
		else if (in_quote && s[i] == in_quote)
			in_quote = 0;
		else
			len++;
		i++;
	}
	if (in_quote)
	{
		ft_putendl_fd(
			"minishell: unexpected EOF while looking for matching quote", 2);
		ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
		*err = 2;
		return (-1);
	}
	return (len);
}

char	*remove_quotes(const char *s)
{
	t_quote_vars	qv;
	int				len;

	qv = (t_quote_vars){0};
	if (!s)
		return (NULL);
	len = count_without_quotes(s, &qv.error);
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


void	process_all_tokens(t_shell *sh, char **envp, int last_status)
{
	int		i;
	char	*cleaned;

	if (!sh || !sh->token || !sh->token->tokens)
		return ;
	sh->token->tokens = expand_token(sh, envp, last_status);
	if (!sh->token->tokens)
		return ;
	i = 0;
	while (sh->token->tokens[i])
	{
		cleaned = remove_quotes(sh->token->tokens[i]);
		free(sh->token->tokens[i]);
		sh->token->tokens[i] = cleaned;
		i++;
	}
}
