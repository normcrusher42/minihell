/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_expander.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 19:18:20 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/18 19:19:08 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_dollar_token(t_expander_ctx *ctx, t_shell *sh)
{
	unsigned char	next;

	next = (unsigned char)ctx->token[ctx->i + 1];
	if (!ft_isascii(next) || !ft_isprint(next))
		return ;
	if (ctx->token[ctx->i + 1] == '$' || ctx->token[ctx->i + 1] == '?')
		ctx->new_token = merge_str(ctx, sh);
	else if (ctx->token[ctx->i + 1])
	{
		if (ft_isdigit(next))
			handle_digit_case(ctx);
		else if (!ft_isalnum(next) && next != '_' && !ft_isspace(next)
			&& next != '\'' && next != '"')
			ctx->new_token = merge_str(ctx, sh);
		else if (next != '\'' && next != '"' && !ft_isspace(next))
			ctx->new_token = env_expander(ctx->token, ctx->merge, ctx->envp,
					ctx->i);
	}
}

// Scans for special characters (including unicode) and decides if expandable.
// Handles the '$' expansion cases
static void	handle_dollar(t_expander_ctx *ctx, t_shell *sh)
{
	free_arr(&ctx->merge, YES);
	process_dollar_token(ctx, sh);
	if (!ctx->new_token)
		return ;
	if (ctx->token)
		free(ctx->token);
	ctx->token = ctx->new_token;
	ctx->i = -1;
	ctx->in_single = 0;
	ctx->in_double = 0;
}

// The '$' condition scanner for $VAR, $?, & $$.
char	*dollar_expander(char *token, char **envp, t_shell *sh)
{
	t_expander_ctx	ctx;

	ctx = (t_expander_ctx){0};
	ctx.i = -1;
	ctx.merge = ft_calloc(4, sizeof(char *));
	if (!ctx.merge)
		return (NULL);
	ctx.token = token;
	ctx.envp = envp;
	while (ctx.token[++ctx.i])
	{
		if (!sh->in_heredoc && ctx.token[ctx.i] == '\'' && !ctx.in_double)
			ctx.in_single = !ctx.in_single;
		else if (!sh->in_heredoc && ctx.token[ctx.i] == '"' && !ctx.in_single)
			ctx.in_double = !ctx.in_double;
		else if (!ctx.in_single && ctx.token[ctx.i] == '$')
			handle_dollar(&ctx, sh);
	}
	free_arr(&ctx.merge, NO);
	return (ctx.token);
}
