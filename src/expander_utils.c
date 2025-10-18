/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 00:58:39 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/18 00:58:39 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The holy grail of expanding valid $ input if the other cases weren't met.
static char	*env_expander(char *token, char **merge, char **envp, int i)
{
	t_expander_ctx	q;

	q.env_values = ft_calloc(3, sizeof(char *));
	if (!q.env_values)
		return (NULL);
	if (token[i] == '$' && (ft_isalpha(token[i + 1]) || token[i + 1] == '_'))
	{
		q.strt = i + 1;
		while (token[q.strt] && (ft_isalnum(token[q.strt])
				|| token[q.strt] == '_'))
			q.strt++;
		merge[0] = ft_substr(token, 0, i);
		q.env_values[0] = ft_substr(token, i + 1, q.strt - (i + 1));
		merge[1] = ft_substr(token, q.strt, ft_strlen(token) - q.strt);
		q.env_values[1] = get_env_value(envp, q.env_values[0]);
		if (!q.env_values[1])
			q.env_values[1] = ft_strdup("");
		else
			q.env_values[1] = ft_strdup(q.env_values[1]);
		q.updated_token = ft_strjoin3(merge[0], q.env_values[1], merge[1]);
		free_arr(&q.env_values, NO);
		return (q.updated_token);
	}
	free_arr(&q.env_values, NO);
	return (ft_strdup(token));
}

// Straight forward merging for cases like special characters or $$ & $?.
char	*merge_str(t_expander_ctx *ctx, t_shell *sh)
{
	ctx->merge[0] = ft_substr(ctx->token, 0, ctx->i);
	if (ctx->token[ctx->i + 1] == '$')
		ctx->merge[1] = ft_substr("miniOdy", 0, 8);
	else if (ctx->token[ctx->i + 1] == '?')
		ctx->merge[1] = ft_itoa(sh->ex_st);
	else
		ctx->merge[1] = ft_strdup("");
	ctx->merge[2] = ft_substr(ctx->token, ctx->i + 2, ft_strlen(ctx->token));
	return (ft_strjoin3(ctx->merge[0], ctx->merge[1], ctx->merge[2]));
}

static void	handle_digit_case(t_expander_ctx *ctx)
{
	ctx->merge[0] = ft_substr(ctx->token, 0, ctx->i);
	ctx->merge[1] = ft_strdup("");
	ctx->merge[2] = ft_substr(ctx->token, ctx->i + 2, ft_strlen(ctx->token));
	ctx->new_token = ft_strjoin3(ctx->merge[0], ctx->merge[1], ctx->merge[2]);
}
