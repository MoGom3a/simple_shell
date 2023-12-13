#include "shell.h"

/**
 * main - Entry point
 * @argc: arguments count
 * @argv: list of arguments
 * Return: 0 on success, 1 on error
 */
int main(int argc, char **argv)
{
	int fd;
	sh_context sh_ctx[] = {INIT_SH_CTX};

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1) /* Failed to read */
		{
			if (errno == EACCES) /* Permission denied */
				exit(126);		 /* Command found but is not executable */
			if (errno == ENOENT) /* No such file or directory */
				print_error(argv);
			return (EXIT_FAILURE);
		}
		sh_ctx->input_fd = fd;
	}

	run_hsh(sh_ctx, argv);
	return (EXIT_SUCCESS);
}
