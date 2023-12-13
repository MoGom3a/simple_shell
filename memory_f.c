#include "shell.h"

/**
 * _memcpy - Copy from memory string of buffer
 * @dest: pointer to the destination string
 * @src: pointer to the source
 * @n: number of bytes to copy
 * Return: Pointer to the memory area (src)
 */
char *_memcpy(char *dest, const char *src, unsigned int n)
{
	unsigned int i;

	for (i = 0; n--; i++)
		dest[i] = src[i];
	return (dest);
}

/**
 * _memset - Fill a string with some characters
 * @s: pointer to the string
 * @c: character to use
 * @n: number of times
 * Return: Pointer to the memory area (s)
 */
char *_memset(char *s, char c, unsigned int n)
{
	unsigned int i = 0;

	if (!s)
		return (NULL);

	while (i < n)
	{
		s[i] = c;
		i++;
	}
	return (s);
}

/**
 * free_ptr - free pointers and set their address to NULL
 * @p: address of the pointer to be freed
 * Return: 1 on success, 0 otherwise.
 */
int free_ptr(void **p)
{
	if (p && *p)
	{
		free(*p);
		*p = NULL;
		return (1);
	}
	return (0);
}

/**
 * free_str - free a string
 * @arr: pointer to pointer of string
 */
void free_str(char **arr)
{
	int i = 0;

	if (!arr)
		return;

	while (arr[i])
		free(arr[i++]);
	free(arr);
}
