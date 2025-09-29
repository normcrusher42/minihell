/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.ae>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 04:41:48 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/29 04:41:48 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Error message for bad export values (whilst pointing to the bad argument).
static void	export_error(char *arg)
{
	ft_putstr_fd("mOdy: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
}

// Validity identifier for the first character of the key.
static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// Prints all environment variables with "declare -x" prepended.
int	ft_export_print(char **envp)
{
	int		i;
	char	*equal;

	i = 0;
	while (envp[i])
	{
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			*equal = '\0';
			ft_printf("declare -x %s=\"%s\"\n", envp[i], equal + 1);
			*equal = '=';
		}
		else
			ft_printf("declare -x %s\n", envp[i]);
		i++;
	}
	return (0);
}

// Splits & stores the key and value to better process the '=' condition.
static t_kv	split_key_value(const char *arg)
{
	t_kv	kv;
	char	*equal;

	kv = (t_kv){0};
	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		kv.key = ft_strdup(arg);
		return (kv);
	}
	kv.has_equal = 1;
	kv.key = ft_substr(arg, 0, equal - arg);
	if (*(equal + 1) != '\0')
		kv.val = ft_strdup(equal + 1);
	return (kv);
}

// A not-so-simple remake of 'export'. Can print declared envs and defines them.
int	ft_export(char **av, char ***envp)
{
	int		i;
	t_kv	kv;

	if (ft_arrlen(av) == 1)
		return (ft_export_print(*envp));
	i = 0;
	while (av[++i])
	{
		if (!is_valid_identifier(av[i]))
		{
			export_error(av[i]);
			continue ;
		}
		kv = split_key_value(av[i]);
		if (!kv.has_equal)
			set_env_value(envp, kv.key, NULL, 1);
		else if (kv.val == NULL)
			set_env_value(envp, kv.key, "", 0);
		else
			set_env_value(envp, kv.key, kv.val, 0);
		free(kv.key);
		if (kv.val)
			free(kv.val);
	}
	return (0);
}
