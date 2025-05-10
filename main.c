#include "hsh.h"

/**
 * _is_whitespace_only - Check if a string contains only whitespace
 * @str: The string to check
 *
 * Return: 1 if the string contains only whitespace, 0 otherwise
 */
static int _is_whitespace_only(const char *str)
{
	while (*str)
	{
		if (!_isspace((unsigned char)*str))
			return (0);
		str++;
	}
	return (1);
}

/**
 * main - Entry point for the shell program
 * @argc: Argument count
 * @argv: Argument vector
 *
 * This function initializes the prompt, processes command line arguments,
 * and enters a loop to read and execute commands.
 *
 * Return: 0 on success, or EXIT_FAILURE on error and last command status
 *
 * Example:
 *   ./hsh
 *   ./hsh "ls -l"
 */
int main(int argc, char **argv)
{
	prompt_t *prompt;
	char ***tokens;
	int status = 0;
	(void)argc;

	/* Initialize the prompt structure */
	prompt = init_prompt();
	if (!prompt)
	{
		perror("Failed to initialize prompt");
		return (EXIT_FAILURE);
	}

	/* Handle non-interactive mode with command line argument */
	if (argv[1])
	{
		/* Check if argument is empty or just whitespace */
		if (!argv[1][0] || _is_whitespace_only(argv[1]))
		{
			free_prompt(prompt);
			return (0);
		}

		tokens = tokenize_command(argv[1]);
		if (!tokens)
		{
			perror("Failed to tokenize command");
			free_prompt(prompt);
			return (EXIT_FAILURE);
		}
		status = interpret_tokens(tokens, argv[0], 0);
		free_tokens(tokens);
		free_prompt(prompt);
		return (status);
	}

	/* Handle non-interactive mode with stdin input */
	if (!isatty(STDIN_FILENO))
	{
		char *buffer = NULL;
		size_t buffer_size = 0;
		ssize_t read_bytes;

		/* Read first line which should contain our command */
		read_bytes = getline(&buffer, &buffer_size, stdin);
		if (read_bytes > 0)
		{
			/* Remove newline if present */
			if (buffer[read_bytes - 1] == '\n')
				buffer[read_bytes - 1] = '\0';

			/* Process the command */
			tokens = tokenize_command(buffer);
			if (tokens)
			{
				status = interpret_tokens(tokens, argv[0], 0);
				free_tokens(tokens);
			}
		}
		free(buffer);
		free_prompt(prompt);
		return (status);
	}

	/* Interactive shell mode */
	while (get_prompt(prompt) != -1)
	{
		/* Skip empty lines or whitespace-only lines */
		if (!prompt->input || prompt->size == 0 || _is_whitespace_only(prompt->input))
			continue;

		tokens = tokenize_command(prompt->input);
		if (!tokens)
		{
			fprintf(stderr, "%s: line %d: Failed to tokenize command\n",
				argv[0], prompt->line_count);
			continue;
		}

		status = interpret_tokens(tokens, argv[0], prompt->line_count);
		free_tokens(tokens);
	}

	/* If using interactive mode, print a newline before exiting */
	if (isatty(STDIN_FILENO))
		write(STDOUT_FILENO, "\n", 1);

	/* Clean up and return the status of the last command */
	free_prompt(prompt);
	return (status);
}
