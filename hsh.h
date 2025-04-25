#ifndef HSH_H
#define HSH_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

/**
 * struct prompt_s - Structure to hold command information
 * @input: The input string for the command
 * @size: The size of the input string
 */
typedef struct prompt_s
{
	char	*input;
	int		size;
} prompt_t;

/* Prompt */
void	*init_prompt(void);
void	*free_prompt(void *prompt);
void	get_prompt(prompt_t *prompt);

/* Handlers */
void	check_alloc(void *ptr);
void	check_retval(int retval, const char *msg);
void	check_null(void *ptr, const char *msg);

#endif /* HSH_H */
