#ifndef HSH_H
#define HSH_H

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PATH_MAX 4096

extern char **environ;

/**
 * struct prompt_s - Structure to hold command information
 * @input: The input string for the command
 * @size: The size of the input string
 * @line_count: The number of lines in the input
 */
typedef struct prompt_s
{
	char	*input;
	int		size;
	int		line_count;
} prompt_t;

/* Prompt */
prompt_t	*init_prompt(void);
void		*free_prompt(void *prompt);
int			get_prompt(prompt_t *prompt);

/* Tokenizer */
char		***tokenize_command(char *input);

/* Interpreter */
int			interpret_tokens(char ***tokens, char *program_name);

/* Builtins */
int			handle_builtin(char **args, int *status);

/* Prohibited functions */
long		_strtol(const char *str, char **endptr, int base);
char		*_strtok_r(char *str, const char *delim, char **saveptr);
int			_strlen(const char *str);
char		*_strdup(char *str);
int			_strcmp(const char *str1, const char *str2);
char		*_strchr(const char *s, int c);
int			_setenv(const char *name, const char *value, int overwrite);

#endif /* HSH_H */
