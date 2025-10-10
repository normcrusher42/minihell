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

// The holy grail of expanding valid $ input if the other cases weren't met.
char	*env_expander(char *token, char **merge, char **envp, int i)
{
	t_expander_ctx	q;

	q.env_values = ft_calloc(3, sizeof(char *));
	if (!q.env_values)
		return (NULL);
	if (token[i] == '$' && (ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
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
static char	*merge_str(t_expander_ctx *ctx)
{
	ctx->merge[0] = ft_substr(ctx->token, 0, ctx->i);
	if (ctx->token[ctx->i + 1] == '$')
		ctx->merge[1] = ft_substr("miniOdy", 0, 8);
	else
		ctx->merge[1] = ft_itoa(g_last_status);
	ctx->merge[2] = ft_substr(ctx->token, ctx->i + 2, ft_strlen(ctx->token));
	return (ft_strjoin3(ctx->merge[0], ctx->merge[1], ctx->merge[2]));
}

// Scans for special characters (including unicode) and decides if expandable.
// Handles the '$' expansion cases
static void	handle_dollar(t_expander_ctx *ctx)
{
	unsigned char	next;

	free_arr(&ctx->merge, YES);
	next = (unsigned char)ctx->token[ctx->i + 1];
	if (ctx->token[ctx->i + 1] == '$' || ctx->token[ctx->i + 1] == '?')
		ctx->new_token = merge_str(ctx);
	else if (ctx->token[ctx->i + 1])
	{
		if (!ft_isalnum(next) && next != '_' && !ft_isspace(next)
			&& next != '\'' && next != '"')
			ctx->new_token = merge_str(ctx);
		else if (next != '\'' && next != '"')
			ctx->new_token = env_expander(ctx->token, ctx->merge, ctx->envp,
					ctx->i);
		else
			return ;
	}
	else
		return ;
	free(ctx->token);
	ctx->token = ctx->new_token;
	ctx->i = -1;
	ctx->in_single = 0;
	ctx->in_double = 0;
}

// The '$' condition scanner for $VAR, $?, & $$.
static char	*dollar_expander(char *token, char **envp)
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
		if (ctx.token[ctx.i] == '\'' && !ctx.in_double)
			ctx.in_single = !ctx.in_single;
		else if (ctx.token[ctx.i] == '"' && !ctx.in_single)
			ctx.in_double = !ctx.in_double;
		else if (!ctx.in_single && ctx.token[ctx.i] == '$')
			handle_dollar(&ctx);
	}
	free_arr(&ctx.merge, NO);
	return (ctx.token);
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
		if (ft_strchr(sh->token->tokens[ctx.i], '$'))
			ctx.result[ctx.i] = dollar_expander(sh->token->tokens[ctx.i], envp);
		else
		{
			ctx.result[ctx.i] = ft_strdup(sh->token->tokens[ctx.i]);
			free(sh->token->tokens[ctx.i]);
		}
		if (!ctx.result[ctx.i])
		{
			free_arr(&ctx.result, NO);
			free_arr(&sh->token->tokens, NO);
			return (NULL);
		}
		ctx.i++;
	}
	ctx.result[ctx.i] = NULL;
	return (ctx.result);
}
