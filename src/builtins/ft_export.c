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

/* The entire following was done by @Nasser */
//	   export_error
//	   sort_env
//	   ft_export_print
//	   split_key_value
//	   ft_export

#include "minishell.h"

// Error message for bad export values (whilst pointing to the bad argument).
static int	export_error(char *arg)
{
	ft_putstr_fd("miniOdy: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd("': not a valid identifier", 2);
	return (1);
}

// Bubble sort for environment variable array.
static void	sort_env(char **envp)
{
	int		n;
	int		i;
	int		j;
	char	*tmp;

	n = ft_arrlen(envp);
	i = 0;
	while (i < n - 1)
	{
		j = 0;
		while (j < n - i - 1)
		{
			if (ft_strcmp(envp[j], envp[j + 1]) > 0)
			{
				tmp = envp[j];
				envp[j] = envp[j + 1];
				envp[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

// Prints all environment variables with "declare -x" prepended in alpha order.
int	ft_export_print(char **envp)
{
	int		i;
	char	*equal;
	char	**envp_copy;

	envp_copy = dup_env(envp);
	if (!envp_copy)
		return (1);
	sort_env(envp_copy);
	i = -1;
	while (envp_copy[++i])
	{
		equal = ft_strchr(envp_copy[i], '=');
		if (equal)
		{
			*equal = '\0';
			ft_printf("declare -x %s=\"%s\"\n", envp_copy[i], equal + 1);
			*equal = '=';
		}
		else
			ft_printf("declare -x %s\n", envp_copy[i]);
	}
	free_arr(&envp_copy, NO);
	return (0);
}

// Splits & stores the key and value to better process the '=' condition.
static void	split_key_value(const char *arg, t_kv *kv)
{
	char	*equal;

	kv->key = NULL;
	kv->val = NULL;
	kv->has_equal = 0;
	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		kv->key = ft_strdup(arg);
		return ;
	}
	kv->has_equal = 1;
	kv->key = ft_substr(arg, 0, equal - arg);
	if (*(equal + 1) != '\0')
		kv->val = ft_strdup(equal + 1);
}

// A not-so-simple remake of 'export'. Can print declared envs and defines them.
int	ft_export(char **av, char ***envp)
{
	t_kv	kv;

	kv = (t_kv){0};
	if (ft_arrlen(av) == 1)
		return (ft_export_print(*envp));
	while (av[++kv.i])
	{
		if (!is_valid_identifier(av[kv.i]))
		{
			kv.exit = export_error(av[kv.i]);
			continue ;
		}
		split_key_value(av[kv.i], &kv);
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
	return (kv.exit);
}
