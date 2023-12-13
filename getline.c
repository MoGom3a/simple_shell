#include "shell.h"

/**
 * get_line - gets a line whitout the newline
 * @sh_ctx:  pointer to the shell cotext struct
 * Return: bytes read
 */
ssize_t get_line(sh_context *sh_ctx)
{
	static char *buffer;
	static size_t i, j, len;
	char *cmd;
	ssize_t rd_count = 0;
	char **cmd_position = &(sh_ctx->current_cmd);

	_putchar(BUFFER_FLUSH); /* Empty buffer for STDOUT */
	rd_count = create_line(sh_ctx, &buffer, &len);

	if (rd_count == EOF) /* End Of File */
	{
		if (is_interactive(sh_ctx))
			_puts_fd("\n", 1);
		return (-1);
	}
	/* Case we have commands left in the chain buffer */
	if (len)
	{
		j = i;
		cmd = buffer + i;
		handle_chain(sh_ctx, buffer, &j, i, len);
		while (j < len) /* move to the end or to (;) */
		{
			if (is_chain(sh_ctx, buffer, &j))
				break;
			j++;
		}

		i = j + 1;
		if (i >= len) /* end of the buffer */
		{
			i = len = 0; /* reset length */
			sh_ctx->cmd_buffer_type = NORMAL_CHAIN;
		}
		*cmd_position = cmd;
		return (_strlen(cmd));
	}

	*cmd_position = buffer;
	return (rd_count);
}

/**
 * create_line - Create buffer for chained commands
 * @sh_ctx: pointer to the shell cotext struct
 * @b: address of buffer
 * @len: address of len var
 * Return: bytes read count
 */
ssize_t create_line(sh_context *sh_ctx, char **b, size_t *len)
{
	size_t cmd_len = 0;
	ssize_t rd_count = 0;

	if (!*len)
	{
		free(*b);
		*b = NULL;
		signal(SIGINT, handle_sigint);
		rd_count = getline(b, &cmd_len, stdin);

		if (rd_count > 0)
		{
			if ((*b)[rd_count - 1] == '\n')
				(*b)[rd_count - 1] = '\0', rd_count--;

			*len = rd_count;
			sh_ctx->cmd_buffer = b;
			sh_ctx->line_count_flag = 1;
			rm_comments(*b);
		}
	}

	return (rd_count);
}

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * @sh_ctx: pointer to the shell cotext struct
 * @b: the memory buffer
 * @idx: address of current position in buf
 * Return: 1 if chain delimeter found, 0 otherwise
 */
int is_chain(sh_context *sh_ctx, char *b, size_t *idx)
{
	size_t i = *idx;

	if (b[i] == '|' && b[i + 1] == '|')
	{
		b[i++] = '\0';
		sh_ctx->cmd_buffer_type = OR_CHAIN; /* with (||) */
	}
	else if (b[i] == '&' && b[i + 1] == '&')
	{
		b[i++] = '\0';
		sh_ctx->cmd_buffer_type = AND_CHAIN; /* with (&&) */
	}
	else if (b[i] == ';')
	{
		b[i] = '\0';
		sh_ctx->cmd_buffer_type = SEMICOLON_CHAIN; /* with (;) */
	}
	else
		return (0);

	*idx = i;
	return (1);
}

/**
 * handle_chain - Check whether to continue chaining or not
 * @sh_ctx: pointer to the shell cotext struct
 * @b: the memory buffer
 * @p: address of current position in buffer
 * @idx: starting position in buffer
 * @l: length of the buffer
 */
void handle_chain(sh_context *sh_ctx, char *b, size_t *p, size_t idx, size_t l)
{
	size_t i = *p;

	if (sh_ctx->cmd_buffer_type == AND_CHAIN && sh_ctx->exec_status)
	{
		b[idx] = 0;
		i = l;
	}
	if (sh_ctx->cmd_buffer_type == OR_CHAIN && !sh_ctx->exec_status)
	{
		b[idx] = 0;
		i = l;
	}

	*p = i;
}

/**
 * handle_sigint - block ctrl-C request
 * @sig_num: the signal number
 */
void handle_sigint(int sig_num)
{
	UNUSED(sig_num);

	_puts_fd("\n", 1);
	_puts_fd("$ ", 1);
	_putchar(BUFFER_FLUSH);
}
