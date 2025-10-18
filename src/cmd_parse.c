/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsahloul <lsahloul@student.42abudhabi.ae>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:48:38 by lsahloul          #+#    #+#             */
/*   Updated: 2025/10/18 18:50:05 by lsahloul         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// The main function to parse the command table from tokens.
int	parse_command_table(t_shell *sh, int *st)
{
	t_parse_ctx	p;

	p.tk = sh->token;
	p.st = st;
	p.is_quoted = NO;
	if (!init_parse_ctx(&p))
		return (0);
	while (p.tk->tokens[++p.i])
		if (!handle_token(&p))
			return (0);
	if (!finalize_segment(&p.arr, &p.n, &p.cur))
	{
		if (p.n == 0)
		{
			return (handle_empty_segment(&p, sh));
		}
		free_cmd_table(sh);
		free_one_cmd(&p.cur);
		return (syntax_err(NULL, p.st, NULL));
	}
	sh->cmds = p.arr;
	sh->ncmd = p.n;
	sh->is_quoted = p.is_quoted;
	return (1);
}

/*							remove this later if done						  */
void	print_cmd_table(t_shell *sh)
{
	int	i;
	int	j;

	if (!sh->cmds)
		return ;
	i = -1;
	while (++i < sh->ncmd)
	{
		ft_printf("cmd[%d]:\n", i);
		j = -1;
		while (++j < sh->cmds[i].ac)
			ft_printf("  av[%d]=`%s`\n", j, sh->cmds[i].av[j]);
		j = -1;
		while (++j < sh->cmds[i].redir_count)
			ft_printf("  redir[%d]=%d `%s` q=%d\n", j,
				sh->cmds[i].redirs[j].type,
				sh->cmds[i].redirs[j].arg,
				sh->cmds[i].redirs[j].is_quoted);
	}
}
