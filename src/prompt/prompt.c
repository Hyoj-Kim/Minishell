#include "minishell.h"

void	prompt_init(t_prompt *prompt)
{
	rl_catch_signals = 0;
	signal(SIGINT, prompt_new_line);
	signal(SIGQUIT, prompt_ignore_signal);
	prompt->user = getenv("USER");
	prompt->ps1 = new_str_join((char *[]){
			SIGN BGRN EIGN,
			prompt->user, "@minishell > "
			SIGN END EIGN, NULL}, '\0');
	prompt->ps2 = new_str_join((char *[]){
			SIGN BGRN EIGN
			"> "
			SIGN END EIGN, NULL}, '\0');
}

/*
	- prompt: not freed; uses static allocation (init)
	- user: stores result of getenv(); should not be freed
	- ps1, ps2: freed
*/
void	del_prompt(t_prompt *prompt)
{
	free(prompt->ps1);
	free(prompt->ps2);
}

static void	prompt_run_line(char *line, t_shell *shell)
{
	t_token			*tokens;
	t_AST_SCRIPT	*script;

	tokens = new_tokens_from_line(line, shell->env);
	free(line);
	if (!tokens)
		return ;
	script = new_script_from_tokens(tokens);
	if (!script)
	{
		env_set_exitcode(shell->env, EXIT_FAILURE);
		return ((void)error_malloc_msg());
	}
	if (expander(script, shell->env) == ERR)
		return ((void)env_set_exitcode(shell->env, EXIT_FAILURE));
	shell_replace_script(shell, script);
	if (DEBUG)
		ast_script_repr(shell->script);
	shell_exec_script(shell);
	shell_clear_script(shell);
}

void	shell_prompt(t_shell *shell)
{
	char		*line;
	t_prompt	*prompt;

	prompt = &shell->prompt;
	line = prompt->line;
	while (true)
	{
		line = readline(prompt->ps1);
		if (is_line_eof(line))
			return (prompt_replace_line_with(line, prompt->ps1, "exit"));
		// else if (is_str_equal(line, "exit"))
		// 	return (prompt_exit(line));
		else if (!is_line_empty(line))
		{
			add_history(line);
			prompt_run_line(line, shell);
		}
	}
}
