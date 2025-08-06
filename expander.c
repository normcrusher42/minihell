#include "minishell.h"

char	*env_expander(char *token, char **merge, char **envp, int offset)
{
	char		*updated_token;
	char		**env_values;
	int			start;

	env_values = malloc(sizeof(char *) * 3);
	if (!env_values)
		return (NULL);
	env_values[2] = NULL;
	if (token[offset] == '$' && (ft_isalnum(token[offset + 1])
			|| token[offset + 1] == '_'))
	{
		start = offset + 1;
		while (token[start] && (ft_isalnum(token[start])
				|| token[start] == '_'))
			start++;
		merge[0] = ft_substr(token, 0, offset);
		env_values[0] = ft_substr(token, offset + 1, start - (offset + 1));
		merge[2] = ft_substr(token, start, ft_strlen(token) - start);
		env_values[1] = get_env_value(envp, env_values[0]);
		if (!env_values[1])
			env_values[1] = env_values[2];
		updated_token = ft_strjoin3(merge[0], env_values[1], merge[2]);
		return (free_arr(&env_values), updated_token);
	}
	return (ft_strdup(token));
}

static char	*merge_str(char *token, int last_status, int i, char **merge)
{
	merge[1] = ft_itoa(last_status);
	merge[0] = ft_substr(token, 0, i);
	merge[2] = ft_substr(token, i + 2, ft_strlen(token));
	return (ft_strjoin3(merge[0], merge[1], merge[2]));
}

char	*dollar_expander(char *token, int last_status, char **envp)
{
	int		i;
	char	*new_token;
	char	**merge;

	i = -1;
	while (token[++i])
	{
		merge = malloc(sizeof(char *) * 4);
		if (!merge)
			return (NULL);
		merge[3] = NULL;
		if (token[i] == '$' && token[i + 1] == '?')
			new_token = merge_str(token, last_status, i, merge);
		else if (token[i] == '$')
			new_token = env_expander(token, merge, envp, i);
		else
			continue ;
		free(token);
		token = new_token;
		free_arr(&merge);
	}
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
