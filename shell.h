#ifndef SHELL_H
#define SHELL_H

/* HEADERS */
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>

/* MACROS */
#define BUFFER_FLUSH (-1)
#define BUFFER_SIZE (1024)
#define UNUSED(x) (void)(x)
#define CONVERT_LOWERCASE (1)
#define CONVERT_FORCE_SIGN (2)
#define OR_CHAIN (1)
#define AND_CHAIN (2)
#define SEMICOLON_CHAIN (3)
#define NORMAL_CHAIN (0)
#define INIT_SH_CTX                                     \
	{                                                   \
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, \
			0, 0, 0, 0, 0, 0, 0, 0                      \
	}

extern char **environ;

/* STRUCTURES */

/**
 * struct shell_list - Singly linked list
 * @n: a number
 * @str: a string
 * @next: points to the next node
 */
typedef struct shell_list
{
	int n;
	char *str;
	struct shell_list *next;
} sh_list;

/**
 * struct shell_context - The context and state of the simple shell program.
 *
 * @file_name: The name of the current program being processed.
 * @cmd_args: Arguments passed with the command.
 * @cmd_buffer: Pointer to command chain buffer for memory management.
 * @current_cmd: The currently entered command generated from getline.
 * @current_path: The path of the current command.
 * @sys_environ: The system environment variables (local copy of environ).
 *
 * @cmd_aliases: List of command aliases.
 * @env_variables: List of environment variables (local copy of environ).
 *
 * @arg_count: Number of arguments in the command.
 * @error_number: Error number associated with the last operation.
 * @env_status: Flag indicating whether the environment has changed.
 * @cmd_buffer_type: Type of command buffer: CMD_type ||, &&, ;
 * @exec_status: Status of the last command execution.
 * @input_fd: File descriptor for input (stdin).
 * @line_count: Line count for tracking input lines.
 * @line_count_flag: Flag indicating whether to display line count.
 *
 * Description:
 * This struct holds information related to command execution, file descriptors
 * environment variables, and other aspects of the shell program.
 */
typedef struct shell_context
{
	char *file_name;
	char **cmd_args;
	char **cmd_buffer;
	char *current_cmd;
	char *current_path;
	char **sys_environ;

	sh_list *cmd_aliases;
	sh_list *env_variables;

	int arg_count;
	int env_status;
	int error_number;
	int cmd_buffer_type;
	int exec_status;
	unsigned int line_count;
	int line_count_flag;
	int input_fd;
} sh_context;

/**
 *struct features - linked list of features
 *@type: the feature command type
 *@f: pointer to the related function
 */
typedef struct features
{
	char *type;
	int (*f)(sh_context *sh_ctx);
} feat_t;

/* PROTOTYPES */

/* Strings 11-f */
int _strlen(const char *s);
char *_strchr(char *s, char c);
char *_strdup(const char *str);
int _strcmp(char *s1, char *s2);
char *_strcpy(char *dest, char *src);
char *_strcat(char *dest, char *src);
char **split_str(char *s, char *d);
char *_strstr(const char *h, const char *n);
int rplc_str(char **old, char *new);
int is_delimiter(char c, char *d);
void rm_comments(char *s);
int is_digit(char c);

/* Prints 7-f */
void _putchar(char c);
void _putchar_err(char c);
void _puts_fd(char *s, int fd);
void _putchar_fd(char c, int fd);
int print_number(int num);
void print_error(char **av);
void print_err_msg(sh_context *sh_ctx, char *msg);

/* Converts 2-f */
char *convert_ltos(long int lnum, int base);
int convert_stoi_safe(char *s);

/* Memory 4-f */
int free_ptr(void **p);
void free_str(char **pps);
char *_memset(char *s, char c, unsigned int n);
char *_memcpy(char *dest, const char *src, unsigned int n);

/* Sh_ctx 5-f */
void reset_sh_ctx(sh_context *sh_ctx);
void init_sh_ctx(sh_context *sh_ctx, char **argv);
sh_list *node_strstr(sh_list *node, char *value, char c);
void free_sh_ctx(sh_context *sh_ctx, int y);
int handle_sh_vars(sh_context *info);

/* Sh_List 9-f */
sh_list *create_node(const char *str, int n);
sh_list *add_node_end(sh_list **head, const char *str, int n);
int del_node_at_index(sh_list **head, unsigned int idx);
ssize_t get_node_index(sh_list *head, sh_list *node);
size_t print_list_str(const sh_list *h);
size_t print_list(const sh_list *h);
size_t list_len(const sh_list *h);
char **list_to_strs(sh_list *h);
void free_list(sh_list **head);

/* Features 11-f */
int get_features(sh_context *sh_ctx);
int sh_cd(sh_context *sh_ctx);
int sh_exit(sh_context *sh_ctx);
int sh_env(sh_context *sh_ctx);
int sh_rmenv(sh_context *sh_ctx);
int sh_setenv(sh_context *sh_ctx);
int sh_alias(sh_context *sh_ctx);
int add_alias(sh_context *sh_ctx, char *str);
int rm_alias(sh_context *sh_ctx, char *str);
int update_sh_alias(sh_context *sh_ctx);
int print_alias(sh_list *node);

/* Path 3-f */
int is_execmd(sh_context *sh_ctx, char *path);
char *_substr(char *src, int start, int stop);
char *get_path(sh_context *sh_ctx, char *src, char *cmd);

/* Sh_env 5-f */
int fill_shell_env(sh_context *sh_ctx);
char **_get_sys_env(sh_context *sh_ctx);
char *_get_sh_env(sh_context *sh_ctx, const char *name);
int set_sh_env(sh_context *sh_ctx, char *env, char *value);
int unset_sh_env(sh_context *sh_ctx, char *env);

/* Getline 5-f */
ssize_t get_line(sh_context *sh_ctx);
int is_chain(sh_context *sh_ctx, char *b, size_t *idx);
ssize_t create_line(sh_context *sh_ctx, char **b, size_t *len);
void handle_chain(sh_context *sh_ctx, char *b, size_t *p, size_t i, size_t l);
void handle_sigint(int sig_num);

/* Hsh 5-f */
void get_cmd(sh_context *sh_ctx);
void call_fork(sh_context *sh_ctx);
int is_interactive(sh_context *sh_ctx);
int run_hsh(sh_context *sh_ctx, char **argv);
int _hsh_loop(sh_context *sh_ctx, char **argv);

#endif /* Total 67-f */
