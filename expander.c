#include "minishell.h"

char	*merge_strings(char *token, char *frst_str, char *thrd_str)
{
	
}

char	*env_expander(char *token, char *frst_str, char *thrd_str, char **envp)
{
	char	*updated_token;
	char	*var_name;
	char	*value;
	int		i;
	int		start;

	i = -1;
	while (token[++i])
	{
		if (token[i] == '$' && (ft_isalnum(token[i + 1]) || token[i + 1] == '_'))
		{
			start = i + 1;
			while (ft_isalnum(token[start]) || token[start] == '_')
				start++;
			frst_str = ft_substr(token, 0, i);
			var_name = ft_substr(token, i + 1, start - (i + 1));
			thrd_str = ft_substr(token, start, ft_strlen(token));
			value = get_env_value(envp, var_name);
			if (!value)
				value = NULL;
			updated_token = ft_strjoin3(frst_str, value, thrd_str);
		}
	}
}

char	*var_expander(char *token, int last_status, char **envp)
{
	int		i;
	char	*new_token;
	char	*status_str;
	char	*first_str;
	char	*third_str;

	i = -1;
	status_str = ft_itoa(last_status);
	if (!status_str)
		return (NULL);
	while (token[++i])
	{
		if (token[i] == '$' && token[i + 1] == '?')
		{
			first_str = ft_substr(s, 0, i);
			third_str = ft_substr(token, i + 2, ft_strlen(token));
			new_token = ft_strjoin3(first_str, status_str, third_str);
		}
		else
			new_token = env_expander()
	}
	return (free(status_str), free(first_str), free(third_str), new_token);
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
			result[i] = var_expander(token->tokens[i], last_status);
		else
			result[i] = ft_strdup(token->tokens[i]);
		if (!result[i])
			return (NULL);
	}
	free_arr(&token->tokens);
	return (result);
}
