#include "shell.h"

/**
 * is_interactive - Check if shell is in interactive mode
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 1 if in interactive mode, 0 otherwise
 */
int is_interactive(sh_context *sh_ctx)
{
	return (isatty(STDIN_FILENO) && sh_ctx->input_fd <= 2);
}

/**
 * _hsh_loop - .
 * @sh_ctx: pointer to the shell cotext struct
 * @argv: array of args
 * Return: the custom feature return value
 */
int _hsh_loop(sh_context *sh_ctx, char **argv)
{
	int feat_value = 0;
	ssize_t rd_count = 0;

	while (rd_count != -1 && feat_value != -2)
	{
		reset_sh_ctx(sh_ctx);
		if (is_interactive(sh_ctx))
			_puts_fd("$ ", 1);
		_putchar_err(BUFFER_FLUSH);

		rd_count = get_line(sh_ctx);
		if (rd_count != -1)
		{
			init_sh_ctx(sh_ctx, argv);
			update_sh_alias(sh_ctx);
			handle_sh_vars(sh_ctx); /* like => echo $$ */
			feat_value = get_features(sh_ctx);
			if (feat_value == -1) /* No custom feature */
				get_cmd(sh_ctx);
		}

		free_sh_ctx(sh_ctx, 0);
	}

	return (feat_value);
}

/**
 * run_hsh - The main shell execution loop
 * @sh_ctx: pointer to the shell cotext struct
 * @argv: the argument list
 * Return: 0 on success, 1 on failuer or error status
 */
int run_hsh(sh_context *sh_ctx, char **argv)
{
	int feat_value = 0;

	fill_shell_env(sh_ctx);
	feat_value = _hsh_loop(sh_ctx, argv);
	free_sh_ctx(sh_ctx, 1);

	if (!is_interactive(sh_ctx) && sh_ctx->exec_status)
		exit(sh_ctx->exec_status);

	if (feat_value == -2)
	{
		if (sh_ctx->error_number == -1)
			exit(sh_ctx->exec_status);
		exit(sh_ctx->error_number);
	}

	return (feat_value);
}

/**
 * get_cmd - Find a command in the PATH string
 * @sh_ctx: pointer to the shell cotext struct
 */
void get_cmd(sh_context *sh_ctx)
{
	int i, j;
	char *path = NULL;

	sh_ctx->current_path = sh_ctx->cmd_args[0];

	if (sh_ctx->line_count_flag == 1)
	{
		sh_ctx->line_count++;
		sh_ctx->line_count_flag = 0;
	}
	for (i = 0, j = 0; sh_ctx->current_cmd[i]; i++)
		if (!is_delimiter(sh_ctx->current_cmd[i], " \t\n"))
			j++;

	if (!j)
		return;

	path = get_path(sh_ctx, _get_sh_env(sh_ctx, "PATH="), sh_ctx->cmd_args[0]);

	if (path)
		sh_ctx->current_path = path, call_fork(sh_ctx);
	else
	{
		if ((is_interactive(sh_ctx) || _get_sh_env(sh_ctx, "PATH=") ||
			 sh_ctx->cmd_args[0][0] == '/') &&
			is_execmd(sh_ctx, sh_ctx->cmd_args[0]))
			call_fork(sh_ctx);
		else if (*(sh_ctx->current_cmd) != '\n')
			sh_ctx->exec_status = 127, print_err_msg(sh_ctx, "not found\n");
	}
}

/**
 * call_fork - Fork new thread to run cmd
 * @sh_ctx: pointer to the shell cotext struct
 * Return: void
 */
void call_fork(sh_context *sh_ctx)
{
	int exec;
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		_puts_fd("Error fork:", STDERR_FILENO);
		return;
	}
	/* Child process */
	if (pid == 0)
	{
		exec = execve(sh_ctx->current_path, sh_ctx->cmd_args, _get_sys_env(sh_ctx));
		if (exec == -1)
		{
			free_sh_ctx(sh_ctx, 1);
			if (errno == EACCES)
				exit(126); /* Permission denied */
			exit(1);
		}
	}
	/* Parent process */
	else
	{
		wait(&(sh_ctx->exec_status));
		if (WIFEXITED(sh_ctx->exec_status))
		{
			sh_ctx->exec_status = WEXITSTATUS(sh_ctx->exec_status);
			if (sh_ctx->exec_status == 126)
				print_err_msg(sh_ctx, "Permission denied\n");
		}
	}
}
