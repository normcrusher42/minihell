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
		// Copy character as-is
		res[j++] = str[i++];
	}
	res[j] = '\0';
	return (res);
}

void	process_all_tokens(t_token *tok, char **envp, int last_status)
{
	int		i;
	char	*cleaned;

	if (!tok)
		return ;
	tok->tokens = expand_token(tok, envp, last_status);
	if (!tok->tokens)
		return ;
	i = 0;
	while (tok->tokens && tok->tokens[i])
	{
		cleaned = remove_quotes(tok->tokens[i]);
		free(tok->tokens[i]);
		tok->tokens[i++] = cleaned;
	}
}
