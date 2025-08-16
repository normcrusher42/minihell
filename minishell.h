/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: lsahloul <lsahloul@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/07/30 20:37:55 by nanasser          #+#    #+#             */
/*   Updated: 2025/07/31 19:30:13 by lsahloul         ###   ########.fr       */
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
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define RED "\033[0;31m"
# define BRED "\033[1;31m"
# define RESET "\033[0m"

# define YES 1
# define NO 0

extern int	g_last_status; // last exit return value

typedef struct s_cmd
{
	char	**av;
	int		input_fd;
	int		output_fd;
	int		append; // 1 if ">>", 0 if ">"
	int		pipe_in; // 1 if stdin comes from a previous pipe
	int		pipe_out; // 1 if stdout goes to next pipe
	int		is_builtin;
	// t_redir	*redirs;
}	t_cmd;

typedef enum e_quote_type
{
	QTE_NONE,
	QTE_SINGLE,
	QTE_DOUBLE
}	t_quote_type;

typedef struct s_token
{
	char			**tokens; // stores the tokens parsed
	t_quote_type	*quote; // stores parallel quotes in single array
}	t_token;

typedef struct s_shell
{
	char	*input; // ptr to our key inputs for readline
	char	**envp; // environment variable pointer array
	bool	removed; // boolean to check if the envp removed anything
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

int		main(int argc, char **argv, char **envp);
char	*get_env_value(char **envp, const char *key);
void	set_env_value(char ***envp, const char *key, const char *value);
char	**unset_env_value(char **envp, const char *key, t_shell *shell);
int		ft_arrlen(char **arr);
void	free_arr(char ***arr, bool reuse);
void	butter_free(t_shell *shell);
void	execute_command(char *cmd, char **env);
char	*ft_strjoin3(const char *key, const char *input, const char *value);
char	*dollar_expander(char *token, int last_status, char **envp);
int		ft_isspace(int c);
int		execute_job(t_cmd *cmds, int n, t_shell *sh);
int		is_builtin(char *cmd);
int		exec_builtin(char **av, char ***envp);

void	init_signals(void);
void	rl_replace_line(const char *text, int clear_undo);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);
t_token	*tokenize(char *input);
void	free_tokens(t_token *token);

#endif