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
