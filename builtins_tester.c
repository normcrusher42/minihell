






// no. Leen, get out of here. You have no business here. Nu-uh. If u have the time and curiosity, save it for your work, not on mines. (⓿_⓿) //

























































// ....I warned you. There's no escaping what's coming next.//






































 





  / ____|/ __ \  |  _ \   /\   / ____| |/ / |__   __/ __ \  \ \        / / __ \|  __ \| |/ / | | |
 | |  __| |  | | | |_) | /  \ | |    | ' /     | | | |  | |  \ \  /\  / / |  | | |__) | ' /| | | |
 | | |_ | |  | | |  _ < / /\ \| |    |  <      | | | |  | |   \ \/  \/ /| |  | |  _  /|  < | | | |
 | |__| | |__| | | |_) / ____ \ |____| . \     | | | |__| |    \  /\  / | |__| | | \ \| . \|_|_|_|
  \_____|\____/  |____/_/    \_\_____|_|\_\    |_|  \____/      \/  \/   \____/|_|  \_\_|\_(_|_|_)

           ___________    ____
    ______/   \__//   \__/____\		⬜⬜⬜🟥🟥🟥🟥🟥🟥⬜⬜⬜
  _/   \_/  :           //____\\	⬜⬜🟥🟥🟥🟥🟥🟥🟥🟥⬜⬜
 /|      :  :  ..      /        \	⬜🟥🟥🟦🟦🟦🟦🟦🟦🟥🟥⬜
| |     ::     ::      \        /	⬜🟥🟦🟦🟦🟦🟦🟦🟦🟦🟥⬜
| |     :|     ||     \ \______/	⬜🟥🟥🟦🟦🟦🟦🟦🟦🟥🟥⬜
| |     ||     ||      |\  /  |		⬜🟥🟥🟥🟥🟥🟥🟥🟥🟥🟥⬜
 \|     ||     ||      |   / | \	⬜🟥🟥🟥⬜⬜⬜⬜🟥🟥🟥⬜
  |     ||     ||      |  / /_\ \	⬜🟥🟥🟥⬜⬜⬜⬜🟥🟥🟥⬜
  | ___ || ___ ||      | /  /    \	⬜🟥🟥🟥⬜⬜⬜⬜🟥🟥🟥⬜
   \_-_/  \_-_/ | ____ |/__/      \	⬜🟥🟥🟥⬜⬜⬜⬜🟥🟥🟥⬜
                _\_--_/    \      /	⬜🟥🟥🟥⬜⬜⬜⬜🟥🟥🟥⬜
               /____             /	⬜🟥🟥🟥⬜⬜⬜⬜🟥🟥🟥⬜
              /     \           /	⬜🟥🟥🟥⬜⬜⬜⬜🟥🟥🟥⬜
              \______\_________/

















// I tried ¯\_(ツ)_/¯ curiosity has killed the cat

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	shell = (t_shell){0};
	init_shell(envp, &shell);
	init_signals();
	// char *cd_args[]     = {"cd", NULL};
	// char *echo_args[]   = {"echo", "hello", "world", NULL};
	// char *export_args[] = {"export", "FOO=bar", "FOObar=naw", "imagine", NULL};
	// char *unset_args[]  = {"unset", "nothing lol", "FOO", "FOO", "123", "", "@FOO", NULL};
	// char *exit_args[]   = {"exit", "", NULL};
	// printf("== TESTING echo ==\n");
	// ft_echo(echo_args);

	// printf("== TESTING cd + pwd ==\n");
	// ft_cd(cd_args, &shell.envp);
	// ft_pwd();

	// printf("== TESTING export + env ==\n");
	// ft_export(export_args, &shell.envp);
	// ft_export_print(shell.envp);


	// printf("== TESTING unset + env ==\n");
	// ft_unset(unset_args, &shell.envp, &shell);
	// ft_env(shell.envp);

	// printf("== TESTING exit (will terminate!) ==\n");
	// ft_exit(exit_args, &shell);
	// while (1)
	// {
	// 	shell.input = readline("miniOdy$ ");
	// 	if (!shell.input)
	// 		break ;
	// 	if (*shell.input)
	// 		add_history(shell.input);
	// 	butter_free(&shell);
	// }
	// clear_history();
	free_arr(&shell.envp, NO);
	return (0);
}
