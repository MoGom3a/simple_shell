#include "shell.h"

/**
 * get_path - Get the cmd in the source path string
 * @sh_ctx: the sh_ctx struct
 * @src: source to the PATH string
 * @cmd: the cmd to search
 * Return: the path of the cmd or NULL
 */
char *get_path(sh_context *sh_ctx, char *src, char *cmd)
{
	int i = 0, idx = 0;
	char *path;

	if (!src)
		return (NULL);

	if ((_strlen(cmd) > 2) && _strstr(cmd, "./"))
	{
		if (is_execmd(sh_ctx, cmd))
			return (cmd);
	}

	while (1)
	{
		if (!src[i] || src[i] == ':')
		{
			path = _substr(src, idx, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_execmd(sh_ctx, path))
				return (path);
			if (!src[i])
				break;
			idx = i;
		}
		i++;
	}

	return (NULL);
}

/**
 * is_execmd - Check if a file is an executable command
 * @sh_ctx: pointer to the sh_ctx struct
 * @path: pointer to the file path
 * Return: 1 if true, 0 otherwise
 */
int is_execmd(sh_context *sh_ctx, char *path)
{
	struct stat file_st;

	UNUSED(sh_ctx);
	if (!path || stat(path, &file_st))
		return (0);

	if (file_st.st_mode & __S_IFREG)
		return (1);

	return (0);
}

/**
 * _substr - Extracts a substring from a given string, without colons
 * @src: source to the path string
 * @start: starting index
 * @end: ending index
 * Return: pointer to a new buffer
 */
char *_substr(char *src, int start, int end)
{
	int i = 0, j = 0;
	static char buffer[BUFFER_SIZE];

	for (i = start, j = 0; i < end; i++)
		if (src[i] != ':')
			buffer[j++] = src[i];
	buffer[j] = '\0';

	return (buffer);
}
