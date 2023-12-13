#include "shell.h"

/**
 * _strlen - Return the length of string
 * @s: pointer of type char *
 * Return: str_Length
 */
int _strlen(const char *s)
{
	int i = 0;

	while (*s++)
		i++;

	return (i);
}

/**
 * _strstr - Find the first occurrence of the substring (needle) in (haystack)
 * @haystack: pointer type char
 * @needle: pointer type char
 * Return: Pointer to the beginning of the located substring, or NULL
 */
char *_strstr(const char *haystack, const char *needle)
{
	while (*needle)
	{
		if (*needle != *haystack)
			return (NULL);
		needle++;
		haystack++;
	}
	return ((char *)haystack);
}
/**
 * _strcmp - Compare two strings
 * @s1: pointer to the first string
 * @s2: pointer to the second string
 * Return: (0) if equal, (-1) if s1 < s2, (1) if s2 < s1, or the difference
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	if (!*s1 && !*s2)
		return (0);
	if (!*s1) /* s1 is shorter than s2 */
		return (-1);
	if (!*s2) /* s2 is shorter than s1 */
		return (1);

	return (*s1 - *s2);
}

/**
 * _strcat - Append a string to another string
 * @dest: pointer of type char *
 * @src: pointer of type char *
 * Return: the pointer to dest.
 */
char *_strcat(char *dest, char *src)
{
	char *new_dest = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;

	return (new_dest);
}

/**
 * _strcpy - Copy string from src to dest
 * @dest: data of type char *
 * @src: data of type char *
 * Return: the pointer to dest.
 */
char *_strcpy(char *dest, char *src)
{
	int i = 0;

	if (dest == src || *src == '\0')
		return (dest);

	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}

	dest[i] = '\0';
	return (dest);
}
