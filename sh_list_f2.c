#include "shell.h"

/**
 * get_node_index - Get index of a node in the list
 * @h: pointer to the head of the linked list
 * @node:  pointer to the node for which the index needs to be found
 * Return: the index of the node or -1  if not found
 */
ssize_t get_node_index(sh_list *h, sh_list *node)
{
	size_t i = 0;

	while (node)
	{
		if (h == node)
			return (i);
		h = h->next;
		i++;
	}

	return (-1);
}

/**
 * list_len - Get the length of the sh_list
 * @h: pointer to the list
 * Return: length of the list
 */
size_t list_len(const sh_list *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}

	return (i);
}

/**
 * list_to_strs - Convert the sh_list string fields to an array of strings
 * @h: pointer to the first node
 * Return: array of strings or NULL
 */
char **list_to_strs(sh_list *h)
{
	size_t i = 0, len = list_len(h);
	sh_list *n = h;
	char **strs; /* pointer to the new array */

	if (!h || !len)
		return (NULL);

	strs = malloc(sizeof(char *) * (len + 1));
	if (!strs)
		return (NULL);

	while (n)
	{
		size_t str_len = _strlen(n->str); /* get the length of each str field */

		strs[i] = malloc(str_len + 1); /* allocate memory foreach str */
		if (!strs[i])				   /* Failed to allocate the memory */
		{
			while (i > 0)
				free(strs[--i]);
			free(strs);
			return (NULL);
		}
		_strcpy(strs[i], n->str); /* copy the content to the array by indexing */
		n = n->next;
		i++;
	}

	strs[len] = NULL; /* terminate the array with NULL */
	return (strs);
}
