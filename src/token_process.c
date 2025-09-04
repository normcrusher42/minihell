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

static char	*remove_quotes(char *token)
{
	size_t	len;

	len = ft_strlen(token);
	if (len >= 2 && ((token[0] == '\'' && token[len - 1] == '\'')
			|| (token[0] == '"' && token[len - 1] == '"')))
		return (ft_substr(token, 1, len - 2));
	return (ft_strdup(token));
}

void	process_all_tokens(t_token *tok, char **envp, int last_status)
{
	int		i;
	char	*cleaned;

	if (!tok)
		return ;
	tok->tokens = expand_token(tok, envp, last_status);
	i = 0;
	while (tok->tokens && tok->tokens[i])
	{
		cleaned = remove_quotes(tok->tokens[i]);
		free(tok->tokens[i]);
		tok->tokens[i++] = cleaned;
	}
}
