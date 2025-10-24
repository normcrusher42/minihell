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

/* The entire following was done by @Nasser */
//	   prepare_next_call
//	   handle_non_dollar_token
//	   handle_next_char

#include "minishell.h"

void	prepare_next_call(t_expander_ctx *ctx, int next)
{
	if (ctx->new_token != ctx->token)
	{
		free(ctx->token);
		ctx->token = ctx->new_token;
		ctx->new_token = NULL;
	}
	else
		ctx->new_token = NULL;
	if (next != '"' && next != '\'')
	{
		ctx->in_single = 0;
		ctx->in_double = 0;
	}
}

// Handles tokens that do not require dollar expansion.
int	handle_non_dollar_token(t_expander_ctx *ctx, t_token *token,
			t_shell *sh)
{
	ctx->result[ctx->j] = ft_strdup(token->tokens[ctx->i]);
	if (!ctx->result[ctx->j])
	{
		free_arr(&ctx->result, NO);
		free_arr(&token->tokens, NO);
		return (0);
	}
	free(sh->token->tokens[ctx->i]);
	return (1);
}

// Handles the other half of the '$' expansion cases.
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
	else if ((next == '\"' || next == '\'') && !ctx->in_double
		&& !ctx->in_single)
	{
		ctx->merge[0] = ft_substr(ctx->token, 0, ctx->i);
		ctx->merge[1] = ft_strdup("");
		ctx->merge[2] = ft_substr(ctx->token, ctx->i + 1,
				ft_strlen(ctx->token) - (ctx->i + 1));
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
