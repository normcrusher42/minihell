/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 18:46:13 by lsahloul          #+#    #+#             */
/*   Updated: 2025/08/10 19:36:47 by lsahloul         ###   ########.fr       */
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

	i = 0;
	tok->tokens = expand_token(tok, envp, last_status);
	while (tok->tokens && tok->tokens[i])
	{
		cleaned = remove_quotes(tok->tokens[i]);
		free(tok->tokens[i]);
		tok->tokens[i++] = cleaned;
	}
}
