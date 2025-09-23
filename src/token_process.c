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

static void	quote_error(int in_single, int in_double)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for matching `", 2);
	if (in_single)
		ft_putstr_fd("'", 2);
	else if (in_double)
		ft_putstr_fd("\"", 2);
	ft_putendl_fd("'", 2);
	ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
}

char	*remove_quotes(const char *str)
{
	t_quote_vars	qv;

	qv = (t_quote_vars){0};
	qv.res = malloc(ft_strlen(str) + 1);
	if (!qv.res)
		return (NULL);
	qv.i = -1;
	while (str[++qv.i])
	{
		if (str[qv.i] == '\'' && !qv.in_double)
		{
			qv.in_single = !qv.in_single;
			continue ;
		}
		else if (str[qv.i] == '"' && !qv.in_single)
		{
			qv.in_double = !qv.in_double;
			continue ;
		}
		qv.res[qv.j++] = str[qv.i];
	}
	if (qv.in_single || qv.in_double)
		return (quote_error(qv.in_single, qv.in_double), free(qv.res), NULL);
	return (qv.res[qv.j] = '\0', qv.res);
}

void	process_all_tokens(t_shell *sh, char **envp, int last_status)
{
	int		i;
	char	**expanded;
	char	*cleaned;

	if (!sh || !sh->token || !sh->token->tokens)
		return ;
	expanded = expand_token(sh, envp, last_status);
	if (!expanded)
		return ;
	free_arr(&sh->token->tokens, NO);
	sh->token->tokens = expanded;
	i = 0;
	while (sh->token->tokens[i])
	{
		cleaned = remove_quotes(sh->token->tokens[i]);
		free(sh->token->tokens[i]);
		sh->token->tokens[i] = cleaned;
		i++;
	}
}
