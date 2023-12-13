#include "shell.h"

/**
 * _strchr - Locate character and return string from its position
 * @s: pointer to the string
 * @c: character to locate
 * Return: string from the character position
 */
char *_strchr(char *s, char c)
{
	while (*s && *s != c)
		s++;
	return (*s == c ? s : NULL);
}

/**
 * _strdup - Duplicate a string in memory
 * @src: pointer type char
 * Return: NULL if src = NULL | Pointer to a newly allocated space in memory
 */
char *_strdup(const char *src)
{
	char *dup_str;
	int word_len = 0;

	if (src == NULL)
		return (NULL);

	word_len = _strlen(src);
	dup_str = malloc(sizeof(char) * (word_len + 1));

	if (!dup_str)
		return (NULL);

	_memcpy(dup_str, src, word_len + 1);
	return (dup_str);
}

/**
 * count_words - Count  words in a string based on a delimiter string
 * @str: string to get the word count from
 * @delim: the delimiter string
 * Return: 0 or the count
 */
int count_words(char *str, char *delim)
{
	int i, words_count = 0;

	for (i = 0; str[i] != '\0'; i++)
	{
		if (!is_delimiter(str[i], delim) &&
			(is_delimiter(str[i + 1], delim) || !str[i + 1]))
			words_count++;
	}

	return (words_count);
}

/**
 * split_str - Splits a string into words
 * @str: the input string
 * @d: the delimeter string
 * Return: a pointer to an array of strings, or NULL on failure
 */
char **split_str(char *str, char *d)
{
	int i = 0, idx = 0, word_len, words_count;
	char **words;

	if (!str || !*str)
		return (NULL);

	d = d ? d : " "; /* Default delimiter to space */
	words_count = count_words(str, d);
	if (words_count == 0)
		return (NULL);

	words = malloc((1 + words_count) * sizeof(char *));
	if (!words)
		return (NULL);

	for (i = 0; idx < words_count; idx++)
	{
		word_len = 0; /* Reset the length for the next word */

		while (is_delimiter(str[i], d))
			i++; /* skip delimiters */
		while (!is_delimiter(str[i + word_len], d) && str[i + word_len])
			word_len++; /* Calculate the length of the current word */

		words[idx] = malloc((word_len + 1) * sizeof(char));
		if (!words[idx])
		{
			for (i = 0; i < idx; i++)
				free(words[i]);
			free(words);
			return (NULL);
		}
		/* Copy each characters of the current word  */
		_memcpy(words[idx], &str[i], word_len);
		words[idx][word_len] = '\0', i += word_len;
	}
	/* Terminate the words array with NULL */
	words[idx] = NULL;
	return (words);
}
