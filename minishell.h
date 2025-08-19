/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: team                                           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:37:55 by team              #+#    #+#             */
/*   Updated: 2025/08/10 21:00:00 by team             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>
# include <errno.h>
# include "libft/libft.h"
# include "libft/ft_printf/ft_printf.h"
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>

/* Colors (optional) */
# define RED   "\033[0;31m"
# define BRED  "\033[1;31m"
# define RESET "\033[0m"

# define YES 1
# define NO  0

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

typedef struct s_shell
{
	char	*input;              /* readline buffer */
	char	**envp;              /* environment array */
	int		last_exit_status;    /* last pipeline exit status */
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

/* Command built by the parser */
typedef struct s_cmd
{
	char	**argv;
	int		argc;
	t_redir	*redirs;
	int		redir_count;
}	t_cmd;

/* Public API for parser */
int		parse_command_table(t_token *tk, t_cmd **out, int *count, int *st);
void	free_cmd_table(t_cmd *cmds, int n);
void	print_cmd_table(t_cmd *cmds, int n);

/* ===================== */

/* env_utils.c */
char	*get_env_value(char **envp, const char *key);
void	set_env_value(char ***envp, const char *key, const char *value);
char	**unset_env_value(char **envp, const char *key, t_shell *shell);
int		ft_arrlen(char **arr);
void	free_arr(char ***arr, bool reuse);
char	*ft_strjoin3(const char *a, const char *b, const char *c);

/* utils.c */
int		ft_isspace(int c);

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

/* executor.c (stub for now) */
void	execute_command(char *cmd, char **env);

/* main.c */
int		main(int argc, char **argv, char **envp);
void	butter_free(t_shell *shell);

#endif
