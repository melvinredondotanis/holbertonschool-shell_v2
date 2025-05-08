#include "hsh.h"

/**
 * init_prompt - Initialize a prompt structure
 *
 * This function allocates memory for a prompt_t structure and initializes
 * its members.
 *
 * Example:
 *   prompt_t *my_prompt = init_prompt();
 *   if (my_prompt == NULL)
 *     handle_error();
 *
 * Return: Pointer to the initialized prompt_t structure, or NULL on failure
 */
prompt_t *init_prompt(void)
{
	prompt_t *prmt;

	prmt = malloc(sizeof(prompt_t));
	if (!prmt)
		return (NULL);

	prmt->input = NULL;
	prmt->size = 0;
	prmt->line_count = 0;
	return (prmt);
}

/**
 * free_prompt - Free memory allocated for a prompt structure
 * @prmt: Pointer to the prompt structure to free
 *
 * This function frees the memory allocated for the input string and the
 * prompt structure itself.
 *
 * Example:
 *   prompt_t *my_prompt = init_prompt();
 *   // use my_prompt...
 *   free_prompt(my_prompt);
 *   my_prompt = NULL;
 *
 * Return: Always NULL
 */
void *free_prompt(void *prmt)
{
	prompt_t *p;

	p = (prompt_t *)prmt;
	if (p->input)
		free(p->input);
	free(p);
	return (NULL);
}

/**
 * get_prompt - Display a prompt and read user input
 * @prmt: Pointer to the prompt structure
 *
 * This function displays a prompt to the user, reads input from stdin,
 * and stores it in the prompt structure.
 *
 * Example:
 *   // User enters "ls -l" at the prompt
 *   get_prompt(my_prompt);
 *   // my_prompt->input now contains "ls -l"
 *   // my_prompt->size contains 5
 *
 * Return: 0 on success, -1 on failure or EOF
 */
int get_prompt(prompt_t *prmt)
{
	size_t bufsize;
	ssize_t characters;

	bufsize = 0;
	if (prmt->input)
	{
		free(prmt->input);
		prmt->input = NULL;
	}

	printf("$ ");
	fflush(stdout);
	characters = getline(&(prmt->input), &bufsize, stdin);

	if (characters == -1)
		return (-1);

	if (characters > 0 && prmt->input[characters - 1] == '\n')
		prmt->input[characters - 1] = '\0';

	prmt->size = strlen(prmt->input);
	return (0);
}
