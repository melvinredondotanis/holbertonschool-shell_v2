#include "hsh.h"

/**
 * builtin_setenv - Handle the setenv built-in command
 * @args: Arguments passed to the setenv command
 * @program_name: Name of the shell program
 * @line_count: Current line count for error messages
 *
 * Example:
 *   setenv VARIABLE VALUE  - Sets environment variable
 *
 * Return: 0 on success, -1 on failure
 */
int builtin_setenv(char **args, char *program_name, int line_count)
{
	char *name, *value;

	/* Check if both name and value are provided */
	if (!args[1] || !args[2])
	{
		fprintf(stderr, "%s: %d: setenv: Too few arguments\n",
				program_name, line_count);
		return (-1);
	}

	name = args[1];
	value = args[2];

	/* Validate variable name - must start with a letter or underscore */
	if (!((name[0] >= 'a' && name[0] <= 'z') ||
		  (name[0] >= 'A' && name[0] <= 'Z') ||
		  name[0] == '_'))
	{
		fprintf(stderr, "%s: %d: setenv: Invalid variable name\n",
				program_name, line_count);
		return (-1);
	}

	/* Update environment variable */
	if (_setenv(name, value) == -1)
	{
		fprintf(stderr, "%s: %d: setenv: Failed to set environment variable\n",
				program_name, line_count);
		return (-1);
	}

	return (0);
}

/**
 * builtin_unsetenv - Handle the unsetenv built-in command
 * @args: Arguments passed to the unsetenv command
 * @program_name: Name of the shell program
 * @line_count: Current line count for error messages
 *
 * Example:
 *   unsetenv VARIABLE  - Removes environment variable
 *
 * Return: 0 on success, -1 on failure
 */
int builtin_unsetenv(char **args, char *program_name, int line_count)
{
	char *name;
	size_t name_len;
	int i, j;

	/* Check if variable name is provided */
	if (!args[1])
	{
		fprintf(stderr, "%s: %d: unsetenv: Too few arguments\n",
				program_name, line_count);
		return (-1);
	}

	name = args[1];
	name_len = _strlen(name);

	/* Find and remove the environment variable */
	for (i = 0; environ[i]; i++)
	{
		if (_strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			/* Shift all the subsequent environment variables up */
			for (j = i; environ[j]; j++)
				environ[j] = environ[j + 1];
			return (0);
		}
	}

	/* Variable not found, but still return success */
	return (0);
}
