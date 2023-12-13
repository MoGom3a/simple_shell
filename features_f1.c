#include "shell.h"

/**
 * sh_setenv - Add new environment variable or modify one
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 0 or 1
 */
int sh_setenv(sh_context *sh_ctx)
{
	if (sh_ctx->arg_count != 3)
	{
		_puts_fd("Invalid number of arguements\n", STDERR_FILENO);
		return (1);
	}
	if (set_sh_env(sh_ctx, sh_ctx->cmd_args[1], sh_ctx->cmd_args[2]))
		return (0);

	return (1);
}

/**
 * sh_rmenv - Remove an environment variable
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 0 or 1
 */
int sh_rmenv(sh_context *sh_ctx)
{
	int i = 1;

	if (sh_ctx->arg_count == 1)
	{
		_puts_fd("Too few arguements\n", STDERR_FILENO);
		return (1);
	}
	while (i <= sh_ctx->arg_count)
		unset_sh_env(sh_ctx, sh_ctx->cmd_args[i++]);

	return (0);
}

/**
 * sh_exit - exits the shell
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 0 if the cmd was [exit]
 */
int sh_exit(sh_context *sh_ctx)
{
	int exit_value;

	if (sh_ctx->cmd_args[1]) /* the (exit) arguement */
	{
		exit_value = convert_stoi_safe(sh_ctx->cmd_args[1]);
		if (exit_value == -1)
		{
			sh_ctx->exec_status = 2; /* set status 2 indicating error */
			print_err_msg(sh_ctx, "Illegal number: ");
			_puts_fd(sh_ctx->cmd_args[1], 2), _putchar_err('\n');
			return (1);
		}
		sh_ctx->error_number = exit_value;
		return (-2);
	}
	sh_ctx->error_number = -1; /* set defualt value */
	return (-2);
}

/**
 * sh_cd - Change the current directory of the process
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 1 or 0
 */
int sh_cd(sh_context *sh_ctx)
{
	int chd;
	char *cwd, *dir, buffer[BUFFER_SIZE];

	cwd = getcwd(buffer, BUFFER_SIZE);
	if (!cwd)
		_puts_fd("Failed to get current working dir!\n", 1);
	if (!sh_ctx->cmd_args[1]) /* Case we [cd] with no additional arg */
	{
		dir = _get_sh_env(sh_ctx, "HOME=");
		if (dir)
			chd = chdir(dir);
		else /* Can't get the env (HOME) try to cd to (PWD) or else to (/) */
			dir = _get_sh_env(sh_ctx, "PWD="), chd = chdir(dir ? dir : "/");
	}
	else if (_strcmp(sh_ctx->cmd_args[1], "-") == 0) /* Case of [cd -]*/
	{
		if (_get_sh_env(sh_ctx, "OLDPWD=")) /* Case we've the env (OLDPWD) */
		{
			_puts_fd(_get_sh_env(sh_ctx, "OLDPWD="), 1), _putchar('\n');
			dir = _get_sh_env(sh_ctx, "OLDPWD="), chd = chdir(dir ? dir : "/");
		}
		else
		{
			_puts_fd(cwd, 1), _putchar('\n');
			return (1);
		}
	}
	else /* Case of [cd ./any_other_path] */
		chd = chdir(sh_ctx->cmd_args[1]);
	if (chd == -1) /* Case of error */
	{
		print_err_msg(sh_ctx, "can't cd to ");
		_puts_fd(sh_ctx->cmd_args[1], 2), _putchar_err('\n');
	}
	else
	{
		set_sh_env(sh_ctx, "OLDPWD", _get_sh_env(sh_ctx, "PWD="));
		set_sh_env(sh_ctx, "PWD", getcwd(buffer, BUFFER_SIZE));
	}
	return (0);
}
