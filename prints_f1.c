#include "shell.h"

/**
 * _puts_fd - Print string to stdout
 * @s: pointer to the string
 * @fd: optional arg to print to stderr
 * Return: Nothing
 */
void _puts_fd(char *s, int fd)
{
	int i = 0;

	if (!s)
		return;
	while (s[i] != '\0')
	{
		switch (fd)
		{
		case 1:
			_putchar(s[i++]);
			break;
		case 2:
			_putchar_err(s[i++]);
			break;
		default:
			_putchar_fd(s[i++], fd);
		}
	}
}

/**
 * _putchar - Print to stdout using (write)
 * @c: The character to print
 * Return: Nothing, errno is set automatically
 */
void _putchar(char c)
{
	static int i;
	static char buffer[BUFFER_SIZE];

	if (c == BUFFER_FLUSH || i >= BUFFER_SIZE)
	{
		write(1, buffer, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[i++] = c;
}

/**
 * _putchar_err - Print to stderr using (write)
 * @c: The character to print
 * Return: Nothing, errno is set automatically
 */
void _putchar_err(char c)
{
	static int i;
	static char buffer[BUFFER_SIZE];

	if (c == BUFFER_FLUSH || i >= BUFFER_SIZE)
	{
		write(2, buffer, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[i++] = c;
}

/**
 * _putchar_fd - Print to specifc file descriptor using (write)
 * @c: The character to print
 * @fd: The file descriptor to write to
 * Return: Nothing, errno is set appropriately
 */
void _putchar_fd(char c, int fd)
{
	static int i;
	static char buffer[BUFFER_SIZE];

	if (c == BUFFER_FLUSH || i >= BUFFER_SIZE)
	{
		write(fd, buffer, i);
		i = 0;
	}
	if (c != BUFFER_FLUSH)
		buffer[i++] = c;
}
