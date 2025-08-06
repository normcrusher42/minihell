#include "minishell.h"

char	*env_expander(char *token, char **merge, char **envp)
{
	char		*updated_token;
	char		**env_values;
	int			start;
	int			i;

	env_values = malloc(sizeof(char *) * 3);
	if (!env_values)
		return (NULL);
	env_values[2] = ft_strdup("");
	if (token[0] == '$' && (ft_isalnum(token[1]) || token[1] == '_'))
	{
		i = 1;
		while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
			i++;
		start = i;
		merge[0] = ft_substr(token, 0, 0);
		env_values[0] = ft_substr(token, 1, start - 1);
		merge[2] = ft_substr(token, start, ft_strlen(token) - start);
		env_values[1] = get_env_value(envp, env_values[0]);
		if (!env_values[1])
			env_values[1] = env_values[2];
		updated_token = ft_strjoin3(merge[0], env_values[1], merge[2]);
		return (free_arr(&env_values), updated_token);
	}
	return (ft_strdup(token));
}


char	*var_expander(char *token, int last_status, char **envp)
{
	int		i;
	char	*new_token;
	char	**merge;

	i = -1;
	merge = malloc(sizeof(char *) * 4);
	if (!merge)
		return (NULL);
	merge[3] = ft_strdup("");
	while (token[++i])
	{
		if (token[i] == '$' && token[i + 1] == '?')
		{
			merge[1] = ft_itoa(last_status);
			if (!merge[1])
				return (NULL);
			merge[0] = ft_substr(token, 0, i);
			merge[2] = ft_substr(token, i + 2, ft_strlen(token));
			new_token = ft_strjoin3(merge[0], merge[1], merge[2]);
		}
		else if (token[i] == '$')
			new_token = env_expander(token + i, merge, envp);
		free_arr(&merge);
	}
	return (new_token);
}

char	**expand_token(t_token *token, char **envp, int last_status)
{
	int		i;
	char	**result;

	i = -1;
	result = malloc(sizeof(char *) * ft_arrlen(token->tokens) + 1);
	if (!result)
		return (NULL);
	while (token->tokens[++i])
	{
		if (ft_strchr(token->tokens[i], '$') && token->quote[i] != QTE_SINGLE)
			result[i] = var_expander(token->tokens[i], last_status, envp);
		else
			result[i] = ft_strdup(token->tokens[i]);
		if (!result[i])
			return (free_arr(&result[i]), NULL);
	}
	free_arr(&token->tokens);
	return (result);
}
