#include "hsh.h"

/**
 * main - Entry point of the shell program
 * @argc: Argument count
 * @argv: Argument vector (array of strings)
 * Return: Exit status of the program
 */
int main(int argc, char **argv)
{
	(void)argc;
	int status = 0;
	unsigned int nb_commands = 0;
	prompt_t *prmt = init_prompt();

	if (!isatty(STDIN_FILENO))
	{
		prmt->input = strdup(argv[0]);
		if (!prmt->input)
		{
			perror("strdup");
			free_prompt(prmt);
			return (1);
		}
		prmt->size = strlen(prmt->input);
		status = printf("Command %u: %s\n", nb_commands, prmt->input);
		free_prompt(prmt);
		return (status);
	}

	while (1)
	{
		get_prompt(prmt);
		nb_commands++;
		status = printf("Command %u: %s", nb_commands, prmt->input);
	}
	free_prompt(prmt);

	return (status);
}