#include "minishell.h"

char	*env_expander

char	*var_expander(char *token, int last_status)
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
	if (!new_token)
		return (free(status_str), NULL);
	while (token[++i])
	{
		if (token[i] == '$' && token[i + 1] == '?')
		{
			first_str = ft_substr(s, 0, i);
			third_str = ft_substr(token, i + 2, ft_strlen(token));
			new_token = ft_strjoin3(first_str, status_str, third_str);
		}
		else
		{
			
		}
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
			result[i] = token->tokens[i];
		if (!result[i])
			return (NULL);
	}
	free_arr(&token->tokens);
	return (result);
}
