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
	prompt_t *prmt = init_prompt();
	char *program_name = (argv[0] != NULL) ? argv[0] : "./hsh";

	if (!isatty(STDIN_FILENO))
	{
		size_t bufsize = 0;
		ssize_t characters = getline(&(prmt->input), &bufsize, stdin);

		if (characters == -1)
		{
			free_prompt(prmt);
			return (1);
		}

		if (characters > 0 && prmt->input[characters - 1] == '\n')
			prmt->input[characters - 1] = '\0';

		prmt->size = strlen(prmt->input);
		prmt->line_count = 1;
		status = interpret_command(prmt, program_name);
		free_prompt(prmt);
		return (status);
	}

	while (1)
	{
		if (get_prompt(prmt) == -1)
		{
			if (isatty(STDIN_FILENO))
				printf("\n");
			break;
		}

		if (prmt->input && *(prmt->input))
		{
			prmt->line_count++;
			status = interpret_command(prmt, program_name);
			if (status == 255)
				break;
		}
	}

	free_prompt(prmt);
	return (status);
}