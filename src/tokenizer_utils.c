/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 19:21:36 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/18 19:22:31 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quoted_token(const char *s)
{
	size_t	len;

	if (!s)
		return (0);
	len = ft_strlen(s);
	if (len >= 2 && ((s[0] == '\'' && s[len - 1] == '\'')
			|| (s[0] == '"' && s[len - 1] == '"')))
		return (1);
	return (0);
}

// Checks if the character is an operator.
static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

// Skips over quoted sections in the string
// and returns the number of characters skipped.
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
