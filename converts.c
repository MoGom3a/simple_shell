#include "shell.h"

/**
 * convert_ltos - Convert long integers into a string representation
 * @lnum: long number
 * @base: base value (10, 16, etc...)
 * Return: pointer to string
 */
char *convert_ltos(long int lnum, int base)
{
	static char buf[100];
	unsigned long ulnum = (lnum < 0) ? -lnum : lnum;
	char *arr = (0 & CONVERT_LOWERCASE)
					? "0123456789abcdef"
					: "0123456789ABCDEF";
	char *p = &buf[sizeof(buf) - 1];

	*p = '\0';

	do {
		*--p = arr[ulnum % base];
		ulnum /= base;
	} while (ulnum != 0);

	if (0 & CONVERT_FORCE_SIGN && lnum >= 0)
		*--p = '+';
	if (lnum < 0)
		*--p = '-';

	return (p);
}

/**
 * convert_stoi_safe - converts a string to an integer
 * @s: the string to be converted
 * Return: the converted number, 0 if no numbers found, -1 on error
 */
int convert_stoi_safe(char *s)
{
	unsigned long int result = 0;

	while (*s)
	{
		if (*s == '+')
		{
			s++;
			continue; /* skip this iteration for performance sake */
		}
		if (is_digit(*s))
		{
			result *= 10;
			result += (*s - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
		s++;
	}
	return (result);
}
