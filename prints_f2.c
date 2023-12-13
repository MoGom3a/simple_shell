#include "shell.h"

/**
 * print_number - prints a decimal number to file descriptor
 * @num: number to print
 * Return: number of characters printed
 */
int print_number(int num)
{
	int count = 0;
	int leadingZeros = 1;
	int divisor = 1000000000;

	if (num < 0)
	{
		num = -num;
		_putchar_err('-');
		count++;
	}

	if (num == 0)
	{
		_putchar_err('0');
		return (1);
	}

	while (divisor > 0)
	{
		int digit = num / divisor;

		if (digit > 0 || !leadingZeros)
		{
			_putchar_err('0' + digit);
			count++;
			leadingZeros = 0;
		}
		num %= divisor;
		divisor /= 10;
	}

	return (count);
}

/**
 * print_list - Print all elements of a given sh_list
 * @h: pointer to first node of the list
 * Return: node list count
 */
size_t print_list(const sh_list *h)
{
	size_t nc = 0;
	char *s;

	while (h)
	{
		s = convert_ltos(h->n, 10);
		_puts_fd(s, 1);
		_putchar(':');
		_putchar(' ');
		_puts_fd(h->str ? h->str : "(nil)", 1);
		_puts_fd("\n", 1);
		h = h->next;
		nc++;
	}

	return (nc);
}

/**
 * print_list_str - Print the string field of the list_t struct
 * @h: pointer to first node
 * Return: count of nodes in the list
 */
size_t print_list_str(const sh_list *h)
{
	size_t count = 0;

	while (h)
	{
		_puts_fd(h->str ? h->str : "(nil)", 1);
		_puts_fd("\n", 1);
		h = h->next;
		count++;
	}

	return (count);
}

/**
 * print_error - Print error message to stderr
 * @av: list of args
 */
void print_error(char **av)
{
	_puts_fd(av[0], STDERR_FILENO);
	_puts_fd(": 0: No such file or directory ", STDERR_FILENO);
	_puts_fd(av[1], STDERR_FILENO);
	_putchar_err('\n');
	_putchar_err(BUFFER_FLUSH);
	exit(127);
}

/**
 * print_err_msg - Print a specific error message to stderr
 * @sh_ctx: pointer to the sh_ctx struct
 * @msg: string containing specified error type
 */
void print_err_msg(sh_context *sh_ctx, char *msg)
{
	_puts_fd(sh_ctx->file_name, STDERR_FILENO);
	_puts_fd(": ", STDERR_FILENO);
	print_number(sh_ctx->line_count);
	_puts_fd(": ", STDERR_FILENO);
	_puts_fd(sh_ctx->cmd_args[0], STDERR_FILENO);
	_puts_fd(": ", STDERR_FILENO);
	_puts_fd(msg, STDERR_FILENO);
}
