#include "shell.h"

/**
 * init_sh_ctx - Initialize the shell_context struct
 * @sh_ctx: pointer to the shell_context struct
 * @argv: argument list
 */
void init_sh_ctx(sh_context *sh_ctx, char **argv)
{
	int count = 0;

	sh_ctx->file_name = argv[0];

	if (sh_ctx->current_cmd)
	{
		sh_ctx->cmd_args = split_str(sh_ctx->current_cmd, " \t");
		if (!sh_ctx->cmd_args)
		{
			sh_ctx->cmd_args = malloc(2 * sizeof(char *));
			if (sh_ctx->cmd_args)
			{
				sh_ctx->cmd_args[0] = _strdup(sh_ctx->current_cmd);
				sh_ctx->cmd_args[1] = NULL;
			}
		}
		while (sh_ctx->cmd_args && sh_ctx->cmd_args[count])
			count++;
		sh_ctx->arg_count = count;
	}
}

/**
 * handle_sh_vars - Handle variables in the parsed string
 * @sh_ctx: pointer to the shell context struct
 * Return: 1 if replaced, 0 otherwise
 */
int handle_sh_vars(sh_context *sh_ctx)
{
	int i = 0;
	sh_list *node;

	for (i = 0; sh_ctx->cmd_args[i]; i++)
	{
		if (sh_ctx->cmd_args[i][0] != '$' || !sh_ctx->cmd_args[i][1])
			continue;
		if (_strcmp(sh_ctx->cmd_args[i], "$$") == 0) /* print the pid number */
		{
			rplc_str(&(sh_ctx->cmd_args[i]), _strdup(convert_ltos(getpid(), 10)));
			continue;
		}
		if (_strcmp(sh_ctx->cmd_args[i], "$?") == 0) /* print the exit code */
		{
			rplc_str(&(sh_ctx->cmd_args[i]),
					 _strdup(convert_ltos(sh_ctx->exec_status, 10)));
			continue;
		}
		node = node_strstr(sh_ctx->env_variables, &sh_ctx->cmd_args[i][1], '=');
		if (node)
		{
			rplc_str(&(sh_ctx->cmd_args[i]), _strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		rplc_str(&(sh_ctx->cmd_args[i]), _strdup(""));
	}

	return (0);
}

/**
 * reset_sh_ctx - Reset some fields in the sh_ctx struct
 * @sh_ctx: pointer to the shell_context struct
 */
void reset_sh_ctx(sh_context *sh_ctx)
{
	sh_ctx->arg_count = 0;
	sh_ctx->cmd_args = NULL;
	sh_ctx->current_cmd = NULL;
	sh_ctx->current_path = NULL;
}

/**
 * free_sh_ctx - Free the shell context struct fields
 * @sh_ctx: pointer to the shell context struct
 * @y: optional arg for freeing all fields
 */
void free_sh_ctx(sh_context *sh_ctx, int y)
{
	free_str(sh_ctx->cmd_args);
	sh_ctx->cmd_args = NULL;
	sh_ctx->current_path = NULL;

	if (y)
	{
		if (!sh_ctx->cmd_buffer)
			free(sh_ctx->current_cmd);
		if (sh_ctx->env_variables)
			free_list(&(sh_ctx->env_variables));
		if (sh_ctx->cmd_aliases)
			free_list(&(sh_ctx->cmd_aliases));

		free_str(sh_ctx->sys_environ);
		sh_ctx->sys_environ = NULL;
		free_ptr((void **)sh_ctx->cmd_buffer);

		if (sh_ctx->input_fd > 2)
			close(sh_ctx->input_fd);
		_putchar(BUFFER_FLUSH);
	}
}
