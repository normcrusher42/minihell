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

# define _GNU_SOURCE
# define RL_USE_OLD_TERMIOS
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
# include <termios.h>
# include <sys/stat.h>
# include <fcntl.h>

/* Colors ✨ */
/* Pls use appropriately */
# define RED   "\033[0;31m"
# define BRED  "\033[1;31m"
# define YELLOW  "\033[0;33m"
# define BYELLOW "\033[1;33m"
# define GREEN   "\033[0;32m"
# define BGREEN  "\033[1;32m"
# define BLUE    "\033[0;34m"
# define BBLUE   "\033[1;34m"
# define MAGENTA "\033[0;35m"
# define BMAGENTA "\033[1;35m"
# define CYAN    "\033[0;36m"
# define BCYAN   "\033[1;36m"
# define WHITE   "\033[0;37m"
# define BWHITE  "\033[1;37m"
# define RESET "\033[0m"

# define YES 1
# define NO 0

/* ======================== */
/* === Error Messages ⚠️ ===*/
/* ======================== */
# define UNEXPECTED_EOF "miniOdy: unexpected EOF while looking for matching quot\
e"
# define SYNTAX_ERROR "miniOdy: syntax error: unexpected end of file"

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

/* Token data used by tokenizer */
typedef struct s_token
{
	char	**tokens; // tokens parsed
	int		*quoted;
}	t_token;

/* One redirection entry */
typedef struct s_redir
{
	t_redirtype	type;
	char		*arg; // file or heredoc delimiter
	int			fd;
}	t_redir;

/* One command node (argv + redirections + pipe info) */
typedef struct s_cmd
{
	char	**av;
	int		ac;
	int		input_fd;
	int		output_fd;
	int		redir_count;
	int		append; // 1 if ">>", 0 if ">"
	int		pipe_in; // 1 if stdin comes from previous pipe
	int		pipe_out; // 1 if stdout goes to next pipe
	int		is_builtin;
	int		*quoted;
	t_redir	*redirs;
}	t_cmd;

/* Pipeline context for fork & pipe handling */
typedef struct s_pipeinfo
{
	int		i;
	int		n;
	int		prev_fd;
	int		pipefd[2];
	pid_t	pid;
	int		status;
	pid_t	*pids;
}	t_pipeinfo;

/* Parser working context */
typedef struct s_parse_ctx
{
	t_token	*tk;
	t_cmd	cur;
	t_cmd	*arr;
	int		n;
	int		i;
	int		*st;
	int		is_quoted;
}	t_parse_ctx;

/* Key/Value pair structure for environment variables */
typedef struct s_kv
{
	int		i;
	char	*key;
	char	*val;
	bool	has_equal;
	int		exit;
}	t_kv;

/* Context used during env modification (export/unset) */
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

/* Variables for the expander */
typedef struct s_expander_ctx
{
	int		i;
	int		j;
	int		had_dollar;
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

/* Variables for Quote stripping */
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

/* Variables for execution (path resolution) */
typedef struct s_exec_vars
{
	char	*path_env;
	char	**paths;
	char	*full;
	int		i;
}	t_exec_vars;

/* Global shell state */
typedef struct s_shell
{
	char	*input; // readline buffer
	char	**envp; // environment array
	t_token	*token;
	int		ex_st; // exit status of last command
	t_cmd	*cmds;
	int		ncmd; // num of cmds
	bool	is_child; // bool to make redir funct safer without exiting parent
	int		is_quoted; // heredoc delim quoted? 1/0
	bool	in_heredoc;
	bool	err; // early syntax error flag check to avoid resuming cmd parsing
}	t_shell;

/* Token type enum used by tokenizer */
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
/* === Parser API     ===*/
/* ===================== */
int		parse_command_table(t_shell *sh, int *st);
void	free_cmd_table(t_shell *sh);
void	free_one_cmd(t_cmd *c);
void	free_cmd_table_ctx(t_cmd *cmds, int n);
int		syntax_err(const char *tok, int *st, t_parse_ctx *p);
void	init_cmd(t_cmd *c);

/* ===================== */
/* === Env Utils.c    ===*/
/* ===================== */
char	*get_env_value(char **envp, const char *key);
void	set_env_value(char ***envp, const char *key, const char *val,
			int exist);
char	**unset_env_value(char **envp, const char *key);
int		ft_arrlen(char **arr);
void	free_arr(char ***arr, bool reuse);
char	*ft_strjoin3(const char *key, const char *input, const char *value);

/* ===================== */
/* === Execution API  ===*/
/* ===================== */
int		execute_command(char ***env, t_shell *sh);
int		execute_job(t_shell *sh);
void	try_direct_exec(char **av, char ***env, t_shell *sh);
int		apply_redirections(t_cmd *cmd, t_shell *sh);
int		init_and_exec_builtins(char **av, char ***env, t_shell *sh);
void	exec_external(t_shell *sh, char **av, char ***env);
void	handle_parent(t_pipeinfo *p);
void	update_exit_status(t_shell *sh, int status);
int		create_pipe_and_fork(t_pipeinfo *p, int n, t_shell *sh, pid_t *pid);
char	*resolve_path(char *cmd, char **envp);
int		spawn_pipeline_children(t_cmd *cmds, int n, t_shell *sh, t_pipeinfo *p);
void	run_child(t_cmd *cmd, t_shell *sh, t_pipeinfo *p);
void	close_cmds_heredoc_fds(int n, t_cmd *cmds);
int		hd_exit_status(int status, int fds[2]);
int		error_return_and_close_fds(const char *msg, int n, t_shell *sh);

/* ===================== */
/* === Builtins API   ===*/
/* ===================== */
int		is_builtin(char *cmd);
int		exec_builtin(char **av, char ***envp, t_shell *sh);
int		ft_export_print(char **envp);
int		ft_env(char **envp);
int		ft_cd(char **av, char ***envp);
int		ft_export(char **av, char ***envp);
int		ft_unset(char **av, char ***envp);
int		ft_exit(char **av, t_shell *sh);
int		ft_pwd(t_shell *sh);
int		ft_echo(char **av);
void	update_shlvl(char ***envp);

/* ===================== */
/* === String/Utils   ===*/
/* ===================== */
int		ft_isspace(int c);
int		is_numeric(const char *str);
int		ft_atoll(const char *str, long long *code);
int		is_key_match(const char *env, const char *key);
int		ft_strcmp(const char *s1, const char *s2);
int		is_valid_identifier(const char *s);
int		is_quoted_token(const char *s);
int		is_operator(char c);
int		skip_quotes(char *s, char quote);
void	consume_spaces(char *s, int *i);
char	*remove_quotes(const char *str, t_shell *sh);
char	**realloc_env(char **envp, int extra);

/* ===================== */
/* === Parsing Utils  ===*/
/* ===================== */
int		handle_empty_parse(t_shell *sh, t_parse_ctx *p);
int		init_parse_ctx(t_parse_ctx *p);
int		redir_kind(const char *s);
int		is_pipe(const char *s);
int		push_word(t_cmd *c, const char *w);

/* ===================== */
/* === Signals.c      ===*/
/* ===================== */
void	init_signals(void);
void	rl_replace_line(const char *text, int clear_undo);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
void	signal_config(void);

/* ===================== */
/* === Tokenizer.c    ===*/
/* ===================== */
void	tokenize(char *s, t_shell *sh);
void	free_tokens(t_shell *sh);

/* =============================== */
/* === Expander + Token Process ===*/
/* =============================== */
char	**expand_token(t_shell *sh, char **envp, t_token *token);
void	process_all_tokens(t_shell *sh, char **envp);
char	*dollar_expander(char *token, char **envp, t_shell *sh);
int		handle_non_dollar_token(t_expander_ctx *ctx, t_token *token,
			t_shell *sh);
void	handle_next_char(t_expander_ctx *ctx, unsigned char next, t_shell *sh);
int		handle_heredoc(t_redir *redir, t_shell *sh);
char	*env_expander(char *token, char **merge, char **envp, int i);
char	*merge_str(t_expander_ctx *ctx, t_shell *sh);
void	prepare_next_call(t_expander_ctx *ctx, int next);

/* ===================== */
/* === Janitor funcs  ===*/
/* ===================== */
/* aka cleanup for u boomers */
/* ..we can keep this in the final submission right Leen? */
char	**dup_env(char **envp);
void	butter_free_input(t_shell *shell);
void	call_janitor(t_shell *sh);
void	disable_echoctl(void);

#endif
