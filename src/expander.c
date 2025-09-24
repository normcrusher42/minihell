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
char	*very_specific_expander(char *token, char **merge, char **envp, int i)
{
	unsigned char	next;

	next = (unsigned char)token[i + 1];
	if (!ft_isalnum(next) && next != '_' && !ft_isspace(next))
		return (merge_str(token, -1, i, merge));
	return (env_expander(token, merge, envp, i));
}

static int	append_str(t_quote_vars *qv, char *add)
{
	qv->new_res = ft_strjoin3(qv->res, "", add);
	if (!qv->new_res)
		return (0);
	free(qv->res);
	qv->res = qv->new_res;
	return (1);
}

static int	handle_exit_status(t_quote_vars *qv, int last_status)
{
	qv->tmp = ft_itoa(last_status);
	if (!qv->tmp)
		return (0);
	if (!append_str(qv, qv->tmp))
		return (free(qv->tmp), 0);
	free(qv->tmp);
	qv->i += 2;
	return (1);
}

static int	handle_dollar_dollar(t_quote_vars *qv)
{
	if (!append_str(qv, "miniOdy"))
		return (0);
	qv->i += 2;
	return (1);
}

static int	handle_variable(t_quote_vars *qv, char *str, char **envp)
{
	qv->start = ++qv->i;
	while (ft_isalnum(str[qv->i]) || str[qv->i] == '_')
		qv->i++;
	if (qv->i > qv->start)
	{
		qv->var = ft_substr(str, qv->start, qv->i - qv->start);
		if (!qv->var)
			return (0);
		qv->tmp = get_env_value(envp, qv->var);
		if (qv->tmp && !append_str(qv, qv->tmp))
			return (free(qv->var), 0);
		free(qv->var);
		return (1);
	}
	return (append_str(qv, "$"));
}

static int	handle_char(t_quote_vars *qv, char c)
{
	char	buf[2];

	buf[0] = c;
	buf[1] = '\0';
	return (append_str(qv, buf));
}

// The '$' condition scanner for $VAR, $?, & $$.
char	*dollar_expander(char *str, int last_status, char **envp)
{
	t_quote_vars	qv;

	qv = (t_quote_vars){0};
	qv.res = ft_strdup("");
	if (!qv.res)
		return (NULL);
	while (str[qv.i])
	{
		if (str[qv.i] == '\'' && !qv.in_double)
			qv.in_single = !qv.in_single;
		else if (str[qv.i] == '"' && !qv.in_single)
			qv.in_double = !qv.in_double;
		else if (str[qv.i] == '$' && !qv.in_single)
		{
			if (str[qv.i + 1] == '?' && handle_exit_status(&qv, last_status))
				{ qv.i += 2; continue ; }
			if (str[qv.i + 1] == '$' && handle_dollar_dollar(&qv))
				{ qv.i += 2; continue ; }
			if (handle_variable(&qv, str, envp))
				continue ;
		}
		if (!handle_char(&qv, str[qv.i]))
			return (free(qv.res), NULL);
		qv.i++;
	}
	return (qv.res);
}


// The main expander loop after being parsed by the tokenizer.
char	**expand_token(t_shell *sh, char **envp, int last_status)
{
	int		i;
	char	**result;
	char	*expanded;

	if (!sh->token || !sh->token->tokens)
		return (NULL);
	result = malloc(sizeof(char *) * (ft_arrlen(sh->token->tokens) + 1));
	if (!result)
		return (NULL);
	i = -1;
	while (sh->token->tokens[++i])
	{
		expanded = NULL;
		if (ft_strchr(sh->token->tokens[i], '$'))
			expanded = dollar_expander(sh->token->tokens[i], last_status, envp);
		if (!expanded)
			expanded = ft_strdup(sh->token->tokens[i]);
		result[i] = expanded;
	}
	result[i] = NULL;
	return (result);
}
