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

int	handle_non_dollar_token(t_expander_ctx *ctx, t_token *token,
			t_shell *sh)
{
	ctx->result[ctx->j] = ft_strdup(token->tokens[ctx->i]);
	free(sh->token->tokens[ctx->i]);
	if (!ctx->result[ctx->j])
	{
		free_arr(&ctx->result, NO);
		free_arr(&token->tokens, NO);
		return (0);
	}
	return (1);
}

// Scans for special characters (including unicode) and decides if expandable.
// Handles the '$' expansion cases
void	handle_next_char(t_expander_ctx *ctx, unsigned char next,
			t_shell *sh)
{
	if (ft_isdigit(next))
	{
		ctx->merge[0] = ft_substr(ctx->token, 0, ctx->i);
		ctx->merge[1] = ft_strdup("");
		ctx->merge[2] = ft_substr(ctx->token, ctx->i + 2,
				ft_strlen(ctx->token));
		ctx->new_token = ft_strjoin3(ctx->merge[0], ctx->merge[1],
				ctx->merge[2]);
	}
	else if (!ft_isalnum(next) && next != '_' && !ft_isspace(next)
		&& next != '\'' && next != '"')
		ctx->new_token = merge_str(ctx, sh);
	else if (next != '\'' && next != '"' && !ft_isspace(next))
		ctx->new_token = env_expander(ctx->token, ctx->merge, ctx->envp,
				ctx->i);
}
