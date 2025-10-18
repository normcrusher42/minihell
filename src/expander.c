/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 15:56:58 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/10 15:56:58 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_token(t_expander_ctx *ctx, t_shell *sh, char **envp)
{
	ctx->had_dollar = (ft_strchr(sh->token->tokens[ctx->i], '$') != NULL);
	if (ctx->had_dollar)
		ctx->result[ctx->j] = dollar_expander(sh->token->tokens[ctx->i],
				envp, sh);
	else
	{
		ctx->result[ctx->j] = ft_strdup(sh->token->tokens[ctx->i]);
		free(sh->token->tokens[ctx->i]);
	}
	if (!ctx->result[ctx->j])
		return (0);
	return (1);
}

static void	handle_empty_result(t_expander_ctx *ctx)
{
	if (ctx->had_dollar && ctx->result[ctx->j][0] == '\0')
	{
		free(ctx->result[ctx->j]);
		ctx->result[ctx->j] = NULL;
	}
	else
		ctx->j++;
}

// The main expander loop after being parsed by the tokenizer.
char	**expand_token(t_shell *sh, char **envp)
{
	t_expander_ctx	ctx;

	ctx = (t_expander_ctx){0};
	ctx.result = malloc(sizeof(char *) * (ft_arrlen(sh->token->tokens) + 1));
	if (!ctx.result)
		return (NULL);
	while (sh->token->tokens[ctx.i])
	{
		if (!process_token(&ctx, sh, envp))
		{
			free_arr(&ctx.result, NO);
			free_arr(&sh->token->tokens, NO);
			return (NULL);
		}
		handle_empty_result(&ctx);
		ctx.i++;
	}
	ctx.result[ctx.j] = NULL;
	return (ctx.result);
}
