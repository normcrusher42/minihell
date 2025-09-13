/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nanasser <nanasser@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 19:37:57 by nanasser          #+#    #+#             */
/*   Updated: 2025/09/04 19:37:57 by nanasser         ###   ########.fr       */
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

/* Colors ✨ */
/* Pls use appropriately */
# define RED   "\033[0;31m"
# define BRED  "\033[1;31m"
# define RESET "\033[0m"

# define YES 1
# define NO 0

extern int	g_last_status; // last exit return value

typedef enum e_quote_type
{
	QTE_NONE,
	QTE_SINGLE,
	QTE_DOUBLE
}	t_quote_type;

typedef struct s_token
{
	char			**tokens; /* tokens parsed */
	t_quote_type	*quote;   /* quote type per token */
}	t_token;

// Stores key and value, and a bool check if it has an equal.
typedef struct s_kv
{
	char	*key;
	char	*val;
	bool	has_equal;
}	t_kv;

typedef struct s_shell
{
	char	*input;              /* readline buffer */
	char	**envp;              /* environment array */
	bool	removed;             /* env unset helper flag */
	t_token	token;
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

/* Public API for parser */
int		parse_command_table(t_token *tk, t_cmd **out, int *count, int *st);
void	free_cmd_table(t_cmd *cmds, int n);
void	print_cmd_table(t_cmd *cmds, int n);

/* ===================== */

/* env_utils.c */
char	*get_env_value(char **envp, const char *key);
void	set_env_value(char ***envp, const char *key, const char *val, int exist);
char	**unset_env_value(char **envp, const char *key, t_shell *shell);
int		ft_arrlen(char **arr);
void	free_arr(char ***arr, bool reuse);
int		execute_command(t_cmd *cmd, char ***env, t_shell *sh);
char	*ft_strjoin3(const char *key, const char *input, const char *value);
char	*dollar_expander(char *token, int last_status, char **envp);
char	*ft_strjoin3(const char *a, const char *b, const char *c);

/* utils.c */
int		ft_isspace(int c);
int		execute_job(t_cmd *cmds, int n, t_shell *sh);
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
int	ft_unset(char **av, char ***envp, t_shell *sh);
int	ft_exit(char **av, t_shell *sh);
int	ft_pwd(void);
int	ft_echo(char **av);
void	update_shlvl(char ***envp);

/* signals.c */
void	init_signals(void);
void	rl_replace_line(const char *text, int clear_undo);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

/* tokenizer.c */
t_token	*tokenize(char *input);
void	free_tokens(t_token *token);


/* expander.c + token_process.c */
char	**expand_token(t_token *token, char **envp, int last_status);
char	*dollar_expander(char *token, int last_status, char **envp);
void	process_all_tokens(t_token *tok, char **envp, int last_status);

/* janitor functions */
/* aka cleanup for u boomers */
/* ..we can keep this in the final submission right Leen? */
char	**dup_env(char **envp);
void	butter_free_input(t_shell *shell);
void	call_janitor(t_shell *sh);

#endif
