/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 22:06:10 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 22:06:10 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The holy grail of expanding valid $ input if the other cases weren't met.
char	*env_expander(char *token, char **merge, char **envp, int i)
{
	char		*updated_token;
	char		**env_values;
	int			strt;

	env_values = ft_calloc(3, sizeof(char *));
	if (!env_values)
		return (NULL);
	if (token[i] == '$' && (ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
	{
		strt = i + 1;
		while (token[strt] && (ft_isalnum(token[strt]) || token[strt] == '_'))
			strt++;
		merge[0] = ft_substr(token, 0, i);
		env_values[0] = ft_substr(token, i + 1, strt - (i + 1));
		merge[1] = ft_substr(token, strt, ft_strlen(token) - strt);
		env_values[1] = get_env_value(envp, env_values[0]);
		if (!env_values[1])
			env_values[1] = ft_strdup("");
		else
			env_values[1] = ft_strdup(env_values[1]);
		updated_token = ft_strjoin3(merge[0], env_values[1], merge[1]);
		return (free_arr(&env_values, NO), updated_token);
	}
	return (free_arr(&env_values, NO), ft_strdup(token));
}

// Straight forward merging for cases like special characters or $$ & $?.
static char	*merge_str(char *token, int last_status, int i, char **merge)
{
	merge[0] = ft_substr(token, 0, i);
	if (token[i + 1] == '$')
		merge[1] = ft_substr("miniOdy", 0, 8);
	else if (last_status == -1)
		merge[1] = ft_strdup("");
	else
		merge[1] = ft_itoa(last_status);
	merge[2] = ft_substr(token, i + 2, ft_strlen(token));
	return (ft_strjoin3(merge[0], merge[1], merge[2]));
}

// Scans for special characters (including unicode) and decides if expandable.
static char	*very_specific_expander(char *token, char **merge, char **envp, int i)
{
	unsigned char	next;

	next = (unsigned char)token[i + 1];
	if (!ft_isalnum(next) && next != '_' && !ft_isspace(next))
		return (merge_str(token, -1, i, merge));
	return (env_expander(token, merge, envp, i));
}

// The '$' condition scanner for $VAR, $?, & $$.
typedef struct s_expander_ctx
{
	int		i;
	bool	expansion;
	char	*new_token;
	char	**merge;
	char	**result;
}	t_expander_ctx;

static char	*dollar_expander(char *token, int last_status, char **envp)
{
	t_expander_ctx ctx;

	ctx.i = -1;
	ctx.expansion = false;
	ctx.merge = ft_calloc(4, sizeof(char *));
	if (!ctx.merge)
		return (NULL);
	while (token[++ctx.i])
	{
		if (token[ctx.i] == '\'')
			ctx.expansion = !ctx.expansion;
		else if (!ctx.expansion && token[ctx.i] == '$')
		{
			free_arr(&ctx.merge, YES);
			if (token[ctx.i + 1] == '$' || token[ctx.i + 1] == '?')
				ctx.new_token = merge_str(token, last_status, ctx.i, ctx.merge);
			else if (token[ctx.i + 1])
				ctx.new_token = very_specific_expander(token, ctx.merge, envp, ctx.i);
			else
				continue;
			free(token);
			token = ctx.new_token;
			ctx.i = -1;
		}
	}
	free_arr(&ctx.merge, NO);
	return (token);
}

// The main expander loop after being parsed by the tokenizer.
char	**expand_token(t_shell *sh, char **envp, int last_status)
{
	t_expander_ctx ctx;

	ctx = (t_expander_ctx){0};
	ctx.result = malloc(sizeof(char *) * (ft_arrlen(sh->token->tokens) + 1));
	if (!ctx.result)
		return (NULL);
	while (sh->token->tokens[++ctx.i])
	{
		if (ft_strchr(sh->token->tokens[ctx.i], '$'))
			ctx.result[ctx.i] = dollar_expander(sh->token->tokens[ctx.i], last_status, envp);
		else
		{
			ctx.result[ctx.i] = ft_strdup(sh->token->tokens[ctx.i]);
			free(sh->token->tokens[ctx.i]);
		}
		if (!result[ctx.i])
		{
			free_arr(&ctx.result, NO);
			free_arr(&sh->token->tokens, NO);
			return (NULL);
		}
	}
	ctx.result[ctx.i] = NULL;
	free(sh->token->tokens);
	return (ctx.result);
}
