#include "hsh.h"

/**
 * init_prompt - Initialize a prompt structure
 *
 * This function allocates memory for a prompt_t structure and initializes
 * its members. The input member is set to NULL and the size member is set
 * to 0.
 *
 * Return: Pointer to the initialized prompt_t structure
 */
void *init_prompt(void)
{
	prompt_t *prmt = malloc(sizeof(prompt_t));

	check_alloc(prmt);
	prmt->input = NULL;
	prmt->size = 0;
	return (prmt);
}

/**
 * free_prompt - Free memory allocated for a prompt structure
 * @prmt: Pointer to the prompt structure to free
 *
 * This function frees the memory allocated for the input string and the
 * prompt structure itself.
 *
 * Return: NULL (to match the expected return type)
 */
void *free_prompt(void *prmt)
{
	prompt_t *p = (prompt_t *)prmt;

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
 * and stores it in the prompt structure. It handles memory allocation
 * for the input string and checks for errors.
 *
 * Return: 0 on success, -1 on failure
 */
void get_prompt(prompt_t *prmt)
{
	size_t len = 0;
	ssize_t read;

	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "$ ", 2);

	read = getline(&prmt->input, &len, stdin);
	if (read == -1)
	{
		if (errno == 0)
		{
			free_prompt(prmt);
			exit(EXIT_SUCCESS);
		}
		perror("getline");
		free_prompt(prmt);
		exit(EXIT_FAILURE);
	}
	prmt->size = read;
}
