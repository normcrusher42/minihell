#include "minishell.h"

char	*var_expander(char *token, bool last_status)
{
	int		i;
	char	*new_token;

	token
	i = -1;
	if (last_status >= 0)
		while (token[++i])
		{
			if (token[i] == '$' && )
		}
	return (new_token);
}

char	**expand_token(t_token *token, char **envp, bool last_status)
{
	int		i;
	int		j;
	char	**result;

	i = -1;
	result = malloc(sizeof(char *) * ft_arrlen(token->tokens) + 1);
	if (!result)
		return (NULL);
	while (token->tokens[++i])
	{
		j = -1;
		while (token->tokens[i][++j])
		{
			if (token->tokens[i][j] == '$' && token->tokens[i][j + 1] == '?')
				result[i] = var_expander(token->tokens[i], last_status);
			else if (token->tokens[i][j] == '$' && token->tokens[i][j + 1])
				result[i]= var_expander(token->tokens[i], last_status);
			else
				result[i] = token->tokens[i];
			if (!result[i])
					return (NULL);
		}
	}
	return (result);
}

for each token in tokens:
        if token contains '$' and token not from single quotes:
            result = ""
            i = 0
            while i < len(token):
                if token[i] == '$':
                    if token[i+1] == '?':
                        result += str(last_status)
                        i += 2
                    else:
                        var_name = extract letters/numbers/_
                        val = get_env_value(env, var_name)
                        if val: result += val
                        i += len(var_name) + 1
                else:
                    result += token[i]
                    i++
            replace token with result