#include "hsh.h"

/**
 * free_tokens - Frees the memory allocated for the tokens
 * @tokens: The array of tokens to free
 *
 * Example:
 *   If tokens = [["ls", "-l", NULL], ["|", NULL], ["grep", "file", NULL], NULL]
 *   All memory for this structure will be freed
 */
static void free_tokens(char ***tokens)
{
	int i, j;

	if (!tokens)
		return;

	for (i = 0; tokens[i] != NULL; i++)
	{
		for (j = 0; tokens[i][j] != NULL; j++)
			free(tokens[i][j]);
		free(tokens[i]);
	}
	free(tokens);
}

/**
 * tokenize_args - Tokenizes a command and its arguments
 * @command: The command string to tokenize
 *
 * Example:
 *   Input: "ls -l /tmp"
 *   Output: ["ls", "-l", "/tmp", NULL]
 *
 * Return: A pointer to an array of strings, or NULL on failure
 */
static char **tokenize_args(char *command)
{
	char **args;
	char *token, *str, *saveptr;
	int i, j;

	args = malloc(sizeof(char *) * 64);
	str = _strdup(command);
	if (!args || !str)
	{
		free(args);
		free(str);
		return (NULL);
	}

	i = 0;
	token = _strtok_r(str, " ", &saveptr);
	while (token != NULL)
	{
		args[i] = _strdup(token);
		if (!args[i])
		{
			free(str);
			for (j = 0; j < i; j++)
				free(args[j]);
			free(args);
			return (NULL);
		}
		i++;
		token = _strtok_r(NULL, " ", &saveptr);
	}
	args[i] = NULL;
	free(str);
	return (args);
}

/**
 * handle_operator - Handles an operator token
 * @commands: Array of commands
 * @idx: Current index
 * @op_char: Operator character
 * @is_double: Whether it's a double operator
 *
 * Example:
 *   op_char = '>', is_double = 1
 *   Result: commands[idx] = [">>", NULL]
 *
 * Return: 1 on success, 0 on failure
 */
static int handle_operator(char ***commands, int idx, char op_char, int is_double)
{
	char op[3];

	op[0] = op_char;
	op[1] = is_double ? op_char : '\0';
	op[2] = '\0';

	commands[idx] = malloc(sizeof(char *) * 2);
	if (!commands[idx])
		return (0);

	commands[idx][0] = _strdup(op);
	if (!commands[idx][0])
	{
		free(commands[idx]);
		return (0);
	}
	commands[idx][1] = NULL;
	return (1);
}

/**
 * tokenize_command - Tokenizes a command string with operators
 * @input: The input string to tokenize
 *
 * Example:
 *   Input: "ls -l | grep file > output.txt"
 *   Output: [["ls", "-l", NULL], ["|", NULL], ["grep", "file", NULL],
 *            [">", NULL], ["output.txt", NULL], NULL]
 *
 * Return: A pointer to an array of strings, or NULL on failure
 */
char ***tokenize_command(char *input)
{
	char *input_copy;
	int i, j, start;
	char ***commands;
	char tmp;
	int is_double;

	input_copy = _strdup(input);
	commands = malloc(sizeof(char **) * 64);
	if (!commands || !input_copy)
	{
		free(input_copy);
		free(commands);
		return (NULL);
	}

	i = 0;
	j = 0;
	while (input_copy[j] != '\0')
	{
		/* Skip spaces */
		while (input_copy[j] == ' ')
			j++;
		if (input_copy[j] == '\0')
			break;

		/* Handle operators */
		if (input_copy[j] == '|' || input_copy[j] == '>' || input_copy[j] == '<')
		{
			is_double = (input_copy[j] == '>' || input_copy[j] == '<') &&
				input_copy[j + 1] == input_copy[j];

			if (!handle_operator(commands, i, input_copy[j], is_double))
			{
				free_tokens(commands);
				free(input_copy);
				return (NULL);
			}
			i++;
			j += is_double ? 2 : 1;
		}
		/* Handle regular commands */
		else
		{
			start = j;
			while (input_copy[j] != '\0' &&
				   input_copy[j] != '|' &&
				   input_copy[j] != '>' &&
				   input_copy[j] != '<')
				j++;

			tmp = input_copy[j];
			input_copy[j] = '\0';

			commands[i] = tokenize_args(&input_copy[start]);
			if (!commands[i])
			{
				free_tokens(commands);
				free(input_copy);
				return (NULL);
			}
			i++;
			input_copy[j] = tmp;
		}
	}
	commands[i] = NULL;
	free(input_copy);
	return (commands);
}
