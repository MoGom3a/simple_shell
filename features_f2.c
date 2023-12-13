#include "shell.h"

/**
 * sh_alias - Replicate the built-in (alias) fucitionality
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 0
 */
int sh_alias(sh_context *sh_ctx)
{
	sh_list *n = NULL;
	char *str = NULL;
	int i = 1;

	if (sh_ctx->arg_count == 1)
	{
		n = sh_ctx->cmd_aliases;
		while (n)
		{
			print_alias(n);
			n = n->next;
		}
		return (0);
	}

	while (sh_ctx->cmd_args[i])
	{
		str = _strchr(sh_ctx->cmd_args[i], '=');
		if (str)
			add_alias(sh_ctx, sh_ctx->cmd_args[i]);
		else
			print_alias(node_strstr(sh_ctx->cmd_aliases, sh_ctx->cmd_args[i],
									'='));
		i++;
	}

	return (0);
}

/**
 * add_alias - sets alias to string
 * @sh_ctx: pointer to the shell cotext struct
 * @str: the string alias
 * Return: Always 0 on success, 1 on error
 */
int add_alias(sh_context *sh_ctx, char *str)
{
	char *s;

	s = _strchr(str, '=');
	if (!s)
		return (1);
	if (!*++s)
		return (rm_alias(sh_ctx, str));

	rm_alias(sh_ctx, str);
	return (add_node_end(&(sh_ctx->cmd_aliases), str, 0) == NULL);
}

/**
 * rm_alias - sets alias to string
 * @sh_ctx: pointer to the shell cotext struct
 * @s: the string alias
 * Return: 0 on success, 1 on error
 */
int rm_alias(sh_context *sh_ctx, char *s)
{
	char temp_c;
	int result;
	char *str;

	str = _strchr(s, '=');
	if (!str)
		return (1);

	temp_c = *str;
	*str = 0;
	result = del_node_at_index(&(sh_ctx->cmd_aliases),
							   get_node_index(sh_ctx->cmd_aliases,
											  node_strstr(sh_ctx->cmd_aliases, s,
														  -1)));
	*str = temp_c;
	return (result);
}

/**
 * update_sh_alias - Update an aliase
 * @sh_ctx: pointer to the shell cotext struct
 * Return: 1 or 0 otherwise
 */
int update_sh_alias(sh_context *sh_ctx)
{
	int i;
	char *str;
	sh_list *node;

	for (i = 0; i < 10; i++)
	{
		node = node_strstr(sh_ctx->cmd_aliases, sh_ctx->cmd_args[0], '=');
		if (!node)
			return (0);

		free(sh_ctx->cmd_args[0]);
		str = _strchr(node->str, '=');
		if (!str)
			return (0);

		str = _strdup(str + 1);
		if (!str)
			return (0);

		sh_ctx->cmd_args[0] = str;
	}

	return (1);
}

/**
 * print_alias - prints an alias string
 * @node: the alias node
 * Return: Always 0 on success, 1 on error
 */
int print_alias(sh_list *node)
{
	char *str = NULL, *p = NULL;

	if (node)
	{
		str = _strchr(node->str, '=');
		for (p = node->str; p <= str; p++)
			_putchar(*p);
		_putchar('\'');
		_puts_fd(str + 1, 1);
		_putchar('\'');
		_putchar('\n');
		return (0);
	}

	return (1);
}
