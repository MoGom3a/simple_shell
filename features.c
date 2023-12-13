#include "shell.h"

/**
 * sh_env - Print the current environment variables
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 0
 */
int sh_env(sh_context *sh_ctx)
{
	print_list_str(sh_ctx->env_variables);
	return (0);
}

/**
 * get_features - Get some shell commands features
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 0 on success,-1 if feature not found,
 * 1 if found but wasn't successful,-2 if a feature signal exit()
 */
int get_features(sh_context *sh_ctx)
{
	int i, value = -1;
	feat_t feats[] = {
		{"cd", sh_cd},
		{"exit", sh_exit},
		{"alias", sh_alias},
		{"setenv", sh_setenv},
		{"unsetenv", sh_rmenv},
		{"env", sh_env},
		{NULL, NULL}};

	for (i = 0; feats[i].type; i++)
		if (_strcmp(sh_ctx->cmd_args[0], feats[i].type) == 0)
		{
			sh_ctx->line_count++, value = feats[i].f(sh_ctx);
			break;
		}

	return (value);
}
