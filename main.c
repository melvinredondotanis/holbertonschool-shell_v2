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
		char *line = NULL;
		ssize_t characters;
		int exit_requested = 0;
		
		while ((characters = getline(&line, &bufsize, stdin)) != -1)
		{
			if (characters > 0)
			{
				if (line[characters - 1] == '\n')
					line[characters - 1] = '\0';
				
				if (prmt->input)
				{
					free(prmt->input);
					prmt->input = NULL;
				}
				
				prmt->input = strdup(line);
				if (!prmt->input)
				{
					free(line);
					free_prompt(prmt);
					return (1);
				}
				
				prmt->size = strlen(prmt->input);
				prmt->line_count++;
				
				if (*(prmt->input))
				{
					status = interpret_command(prmt, program_name, &exit_requested);
					if (exit_requested)
						break;
				}
			}
		}
		
		free(line);
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
			int exit_requested = 0;

			prmt->line_count++;
			status = interpret_command(prmt, program_name, &exit_requested);
			if (exit_requested)
				break;
		}
	}

	free_prompt(prmt);
	return (status);
}