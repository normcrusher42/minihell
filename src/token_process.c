/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:06:23 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 22:06:23 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	int		in_single;
	int		in_double;
	char	*res;

	if (!str)
		return (NULL);
	res = malloc(ft_strlen(str) + 1);
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	in_single = 0;
	in_double = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double)
		{
			in_single = !in_single; // toggle single quote mode
			i++;
			continue; // skip the quote char
		}
		else if (str[i] == '"' && !in_single)
		{
			in_double = !in_double; // toggle double quote mode
			i++;
			continue; // skip the quote char
		}
		// copy literal character
		res[j++] = str[i++];
	}
	// error: unclosed quote
	if (in_single || in_double)
	{
		free(res);
		ft_putendl_fd("minishell: unexpected EOF while looking for matching quote", 2);
		ft_putendl_fd("minishell: syntax error: unexpected end of file", 2);
		return (NULL);
	}
	res[j] = '\0';
	return (res);
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
	while (sh->token->tokens && sh->token->tokens[i])
	{
		cleaned = remove_quotes(sh->token->tokens[i]);
		free(sh->token->tokens[i]);
		sh->token->tokens[i++] = cleaned;
	}
}
