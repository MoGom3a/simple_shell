#include "shell.h"

/**
 * fill_shell_env - Fill the shell env variables with the environ values
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 0
 */
int fill_shell_env(sh_context *sh_ctx)
{
	size_t i;
	sh_list *node = NULL;
	/* fill from the end to the head */
	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	sh_ctx->env_variables = node;

	return (0);
}

/**
 * _get_sh_env - Get the value of one of the env variables
 * @sh_ctx: pointer to the shell cotext struct
 * @name: environ variable name
 * Return: pointer to the value or NULL if not found
 */
char *_get_sh_env(sh_context *sh_ctx, const char *name)
{
	char *p;
	sh_list *node = sh_ctx->env_variables;

	while (node)
	{
		p = _strstr(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * _get_sys_env - Get the string array copy of our environ
 * @sh_ctx: pointer to the shell cotext struct
 * Return: shell system environments
 */
char **_get_sys_env(sh_context *sh_ctx)
{
	if (!sh_ctx->sys_environ || sh_ctx->env_status)
	{
		sh_ctx->sys_environ = list_to_strs(sh_ctx->env_variables);
		sh_ctx->env_status = 0;
	}

	return (sh_ctx->sys_environ);
}

/**
 * set_sh_env - Add new environment variable to our local copy, or modify one.
 * @sh_ctx: pointer to the shell cotext struct
 * @env: pointer to the environment variable that is being set
 * @value: pointer to the env value
 * Return: 0 or 1 on failuer
 */
int set_sh_env(sh_context *sh_ctx, char *env, char *value)
{
	sh_list *node;
	char *p, *buffer = NULL;

	if (!env || !value)
		return (0);
	buffer = malloc(_strlen(env) + _strlen(value) + 2);
	if (!buffer)
		return (1);
	/* construct the env string */
	_strcpy(buffer, env);
	_strcat(buffer, "=");
	_strcat(buffer, value);

	node = sh_ctx->env_variables;
	while (node)
	{
		p = _strstr(node->str, env);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buffer;
			sh_ctx->env_status = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(sh_ctx->env_variables), buffer, 0);
	free(buffer);
	sh_ctx->env_status = 1;
	return (0);
}

/**
 * unset_sh_env - Remove an environment variable
 * @sh_ctx: pointer to the shell cotext struct
 * @env: the string env var property
 * Return: env status on success, 0 otherwise
 */
int unset_sh_env(sh_context *sh_ctx, char *env)
{
	char *s;
	size_t i = 0;
	sh_list *node = sh_ctx->env_variables;

	if (!node || !env)
		return (0);

	while (node)
	{
		s = _strstr(node->str, env);
		if (s && *s == '=')
		{
			sh_ctx->env_status = del_node_at_index(&(sh_ctx->env_variables), i);
			node = sh_ctx->env_variables;
			i = 0;
			continue;
		}
		node = node->next;
		i++;
	}
	return (sh_ctx->env_status);
}
