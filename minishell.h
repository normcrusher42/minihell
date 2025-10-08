/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42adbudhabi.ae> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 19:40:56 by nanasser          #+#    #+#             */
/*   Updated: 2025/10/06 19:40:56 by nanasser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/ft_printf/ft_printf.h"
# include "libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include "libft/libft.h"
# include "libft/ft_printf/ft_printf.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
#include <termios.h>
# include <sys/stat.h>

/* Colors ✨ */
/* Pls use appropriately */
# define RED   "\033[0;31m"
# define BRED  "\033[1;31m"
# define RESET "\033[0m"

# define YES 1
# define NO 0

// Error messages
# define UNEXPECTED_EOF "miniOdy: unexpected EOF while looking for matching quot\
e"
# define SYNTAX_ERROR "miniOdy: syntax error: unexpected end of file"

extern int	g_last_status; // last exit return value

/* ===================== */
/* === cmd_table part ===*/
/* ===================== */

/* Redirection types */
typedef enum e_redirtype
{
	R_IN,
	R_OUT,
	R_APP,
	R_HEREDOC
}	t_redirtype;

typedef struct s_token
{
	char			**tokens; /* tokens parsed */
}	t_token;

/* One redirection entry */
typedef struct s_redir
{
	t_redirtype	type;
	char		*arg;       /* file or heredoc delimiter */
	int			is_quoted;  /* heredoc delim quoted? 1/0  */
}	t_redir;

typedef struct s_cmd
{
	char	**av;
	int		ac;
	int		input_fd;
	int		output_fd;
	int		redir_count;
	int		append; // 1 if ">>", 0 if ">"
	int		pipe_in; // 1 if stdin comes from a previous pipe
	int		pipe_out; // 1 if stdout goes to next pipe
	int		is_builtin;
	t_redir	*redirs;
}	t_cmd;

typedef struct s_parse_ctx
{
	t_token	*tk;
	t_cmd	cur;
	t_cmd	*arr;
	int		n;
	int		i;
	int		*st;
}	t_parse_ctx;

// Stores key and value, and a bool check if it has an equal.
typedef struct s_kv
{
	char	*key;
	char	*val;
	bool	has_equal;
}	t_kv;

// Variables for setting/unsetting env values
typedef struct s_envctx
{
	int		i;
	int		j;
	size_t	key_len;
	char	**new_envp;
	char	*key;
	char	*val;
	bool	removed; /* env unset helper flag */
	bool	exist;
}	t_envctx;


// Variables for the expander
typedef struct s_expander_ctx
{
	int		i;
	bool	expansion;
	char	**merge;
	char	*new_token;
	int		last_status;
	char	**envp;
	char	*token;
	char	**result;
	bool	in_single;
	bool	in_double;
	char	*updated_token;
	char	**env_values;
	int		strt;
}	t_expander_ctx;

// Variables for Quote stripping
typedef struct s_quote_vars
{
	int		i;
	int		j;
	int		in_quote;
	char	*res;
	int		start;
	char	*tmp;
	char	*var;
	char	*new_res;
	int		error;
}	t_quote_vars;

typedef struct s_exec_vars
{
	char	*path_env;
	char	**paths;
	char	*full;
	int		i;
}	t_exec_vars;

typedef struct s_shell
{
	char	*input;              /* readline buffer */
	char	**envp;              /* environment array */
	t_token	*token;
    int     ex_st;              /* exit status of last command */
	t_cmd	*cmds;
	int		ncmd;
}	t_shell;

typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC
}	t_token_type;

/* Public API for parser */
int		parse_command_table(t_shell *sh, int *st);
void	free_cmd_table(t_shell *sh);
void	print_cmd_table(t_shell *sh);

/* ===================== */

/* env_utils.c */
char	*get_env_value(char **envp, const char *key);
void	set_env_value(char ***envp, const char *key, const char *val, int exist);
char	**unset_env_value(char **envp, const char *key);
int		ft_arrlen(char **arr);
void	free_arr(char ***arr, bool reuse);
int		execute_command(char ***env, t_shell *sh);
char	*ft_strjoin3(const char *key, const char *input, const char *value);

/* utils.c */
int		ft_isspace(int c);
int		execute_job(t_shell *sh);
int		is_builtin(char *cmd);
int		exec_builtin(char **av, char ***envp, t_shell *sh);
char	**realloc_env(char **envp, int extra);
int		is_numeric(const char *str);
int		ft_atoll(const char *str, long long *code);
int	is_key_match(const char *env, const char *key);
int	ft_export_print(char **envp);
int	ft_env(char **envp);
int	ft_cd(char **av, char ***envp);
int	ft_export(char **av, char ***envp);
int	ft_unset(char **av, char ***envp);
int	ft_exit(char **av, t_shell *sh);
int	ft_pwd(void);
int	ft_echo(char **av);
void	update_shlvl(char ***envp);
char	*remove_quotes(const char *str);
int		ft_strcmp(const char *s1, const char *s2);
void	try_direct_exec(char **av, char ***env, t_shell *sh);


/* signals.c */
void	init_signals(void);
void	rl_replace_line(const char *text, int clear_undo);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

/* tokenizer.c */
void	tokenize(char *s, t_shell *sh);
void	free_tokens(t_shell *sh);


/* expander.c + token_process.c */
char	**expand_token(t_shell *sh, char **envp);
void	process_all_tokens(t_shell *sh, char **envp);

/* janitor functions */
/* aka cleanup for u boomers */
/* ..we can keep this in the final submission right Leen? */
char	**dup_env(char **envp);
void	butter_free_input(t_shell *shell);
void	call_janitor(t_shell *sh);
void	disable_echoctl(void);
#endif
