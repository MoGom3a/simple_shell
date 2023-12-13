#include "shell.h"

/**
 * is_digit - Check if a given character is a digit number
 * @c: the char to check for
 * Return: 1 or 0 if false
 */
int is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

/**
 * is_delimiter - Check if a given character is a delimeter
 * @c: the char to check for
 * @d: the delimeter string
 * Return: 1 or 0 if false
 */
int is_delimiter(char c, char *d)
{
	while (*d)
	{
		if (c == *d++)
			return (1);
	}
	return (0);
}

/**
 * rplc_str - Replace a string with a new one
 * @old: the old string
 * @new: the new string
 * Return: 1 if replaced, 0 otherwise
 */
int rplc_str(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

/**
 * rm_comments - Terminate strings that start with the '#' symple with '\0'
 * @s: address of the string to modify
 * Return: Nothing
 */
void rm_comments(char *s)
{
	int i;

	for (i = 0; s[i] != '\0'; i++)
		if (s[i] == '#' && (s[i - 1] == ' ' || !i))
		{
			s[i] = '\0';
			break;
		}
}
