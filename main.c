#include "hsh.h"

/**
 * main - Entry point for the shell program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: 0 on success, or an error code
 */
int main(int argc, char **argv)
{
	prompt_t *prompt;
	char ***tokens;
	int status = 0;
	(void)argc;

	prompt = init_prompt();
	if (!prompt)
	{
		perror("Failed to initialize prompt");
		return (EXIT_FAILURE);
	}

	if (argv[1])
	{
		tokens = tokenize_command(argv[1]);
		if (!tokens)
		{
			free_prompt(prompt);
			return (EXIT_FAILURE);
		}
		status = interpret_tokens(tokens, argv[1]);
		free_prompt(prompt);
		return (status);
	}

	while (get_prompt(prompt) != EOF)
	{
		tokens = tokenize_command(prompt->input);
		if (!tokens)
			continue;

		status = interpret_tokens(tokens, argv[0]);
		free(tokens);
	}

	free_prompt(prompt);
	return (status);
}
