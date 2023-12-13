#include "shell.h"

/**
 * create_node - Create a new node using the list struct
 * @str: pointer to the string field in list
 * @n: the node idx
 * Return: pointer to the list
 */
sh_list *create_node(const char *str, int n)
{
	sh_list *new_node = malloc(sizeof(sh_list));

	if (!new_node)
		return (NULL);

	new_node->n = n ? n : 0;
	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	new_node->next = NULL;

	return (new_node);
}

/**
 * add_node_end - Add new node at the end of a list
 * @head: pointer to a pointer to the list
 * @str: pointer to the string field in the node
 * @n: the node idx
 * Return: pointer to the list
 */
sh_list *add_node_end(sh_list **head, const char *str, int n)
{
	sh_list *node = create_node(str, n);
	sh_list *current = *head;

	if (!node || !head)
		return (NULL);

	if (current)
	{
		while (current->next)
			current = current->next;
		current->next = node;
	}
	else
		*head = node;

	return (node);
}

/**
 * del_node_at_index - Delete a node by a given index
 * @head: pointer to the pointer of listint
 * @idx: the index in listint to insert the new node
 * Return: 1 on success, -1 on failuer
 */
int del_node_at_index(sh_list **head, unsigned int idx)
{
	unsigned int i;
	sh_list *prev = NULL, *cur = *head;

	if (!head || !*head)
		return (0);
	/* Check for invalid index */
	if (idx == 0)
		*head = cur->next; /* Delete head node */
	else
	{
		for (i = 0; cur && i < idx; ++i)
		{
			prev = cur;
			cur = cur->next;
		}

		if (!cur)
			return (0); /* Index out of bounds */
	}
	/* Remove node from list */
	if (prev)
		prev->next = cur->next;
	/* Free memory */
	free(cur->str);
	free(cur);

	return (1);
}

/**
 * node_strstr - Get node from the sh_list whose string starts with the value
 * @node: pointer to the shell list head
 * @value: string to match
 * @c: the next character after the value to match
 * Return: the matching node or NULL
 */
sh_list *node_strstr(sh_list *node, char *value, char c)
{
	char *s = NULL;

	while (node)
	{
		s = _strstr(node->str, value);
		if (s && ((*s == c) || (c == -1)))
			return (node);
		node = node->next;
	}

	return (NULL);
}

/**
 * free_list - Free allocated memory for the sh_list struct
 * @head: pointer to the list
 * Return: Nothing
 */
void free_list(sh_list **head)
{
	sh_list *current, *next;

	if (!head || !*head)
		return;

	current = *head;
	while (current)
	{
		next = current->next;
		free(current->str);
		free(current);
		current = next;
	}

	*head = NULL;
}
