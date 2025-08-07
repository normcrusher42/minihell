#include "minishell.h"

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
		return (free_arr(&env_values), updated_token);
	}
	return (free_arr(&env_values), ft_strdup(token));
}

static char	*merge_str(char *token, int last_status, int i, char **merge)
{
	merge[0] = ft_substr(token, 0, i);
	merge[1] = ft_itoa(last_status);
	merge[2] = ft_substr(token, i + 2, ft_strlen(token));
	return (ft_strjoin3(merge[0], merge[1], merge[2]));
}

char	*dollar_expander(char *token, int last_status, char **envp)
{
	int		i;
	char	*new_token;
	char	**merge;

	i = -1;
	merge = NULL;
	while (token[++i])
	{
		if (!merge)
			merge = ft_calloc(4, sizeof(char *));
		if (!merge)
			return (NULL);
		if (token[i] == '$' && token[i + 1] && token[i + 1] == '?')
			new_token = merge_str(token, last_status, i, merge);
		else if (token[i] == '$' && token[i + 1] && token[i + 1] != '$')
			new_token = env_expander(token, merge, envp, i);
		// else if (token[i] == '$' && token[i + 1] && token[i + 1] == '$')
		// 	new_token = 
		else
			continue ;
		free(token);
		token = new_token;
		free_arr(&merge);
		i = -1;
	}
	if (merge)
		free_arr(&merge);
	return (new_token);
}

char	**expand_token(t_token *token, char **envp, int last_status)
{
	int		i;
	char	**result;

	i = -1;
	result = malloc(sizeof(char *) * (ft_arrlen(token->tokens) + 1));
	if (!result)
		return (NULL);
	while (token->tokens[++i])
	{
		if (ft_strchr(token->tokens[i], '$') && token->quote[i] != QTE_SINGLE)
			result[i] = dollar_expander(token->tokens[i], last_status, envp);
		else
			result[i] = ft_strdup(token->tokens[i]);
		if (!result[i])
			return (free_arr(&result), NULL);
	}
	free_arr(&token->tokens);
	return (result);
}
