#include "hsh.h"

/**
 * free_tokens - Frees the memory allocated for the tokens
 * @tokens: The array of tokens to free
 *
 * Example:
 *   If tokens = [["ls", "-l", NULL], ["|", NULL], ["grep", "file", NULL], NULL]
 *   All memory for this structure will be freed
 */
void free_tokens(char ***tokens)
{
	int i = 0, j;

	if (!tokens)
		return;

	for (; tokens[i] != NULL; i++)
	{
		/* Free each token string in the current command */
		for (j = 0; tokens[i][j] != NULL; j++)
			free(tokens[i][j]);
		/* Free the current command array */
		free(tokens[i]);
	}
	/* Free the main tokens array */
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
	char **args, *token, *str, *saveptr;
	int i = 0, j;

	/* Check for NULL input */
	if (!command)
		return (NULL);

	/* Allocate memory for arguments array and duplicate command string */
	args = malloc(sizeof(char *) * MAX_TOKENS);
	str = _strdup(command);
	if (!args || !str)
	{
		free(args);
		free(str);
		return (NULL);
	}

	token = _strtok_r(str, " \t", &saveptr);
	while (token != NULL && i < MAX_TOKENS - 1)
	{
		/* Duplicate each token and store in args array */
		args[i] = _strdup(token);
		if (!args[i])
		{
			/* Clean up if duplication fails */
			free(str);
			for (j = 0; j < i; j++)
				free(args[j]);
			free(args);
			return (NULL);
		}
		i++;
		token = _strtok_r(NULL, " \t", &saveptr);
	}
	/* Terminate args array with NULL */
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

	/* Validate input parameters */
	if (!commands || idx < 0 || idx >= MAX_TOKENS - 1)
		return (0);

	/* Create operator string (single or double character) */
	op[0] = op_char;
	op[1] = is_double ? op_char : '\0';
	op[2] = '\0';

	/* Allocate memory for operator command array */
	commands[idx] = malloc(sizeof(char *) * 2);
	if (!commands[idx])
		return (0);

	/* Copy operator string and set NULL terminator */
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
	char *input_copy, ***commands, tmp;
	int i = 0, j = 0, start, is_double;

	/* Check for NULL input */
	if (!input)
		return (NULL);

	/* Create a copy of input and allocate memory for commands array */
	input_copy = _strdup(input);
	commands = malloc(sizeof(char **) * MAX_TOKENS);
	if (!commands || !input_copy)
	{
		free(input_copy);
		free(commands);
		return (NULL);
	}

	while (input_copy[j] != '\0' && i < MAX_TOKENS - 1)
	{
		/* Skip spaces */
		while (input_copy[j] == ' ' || input_copy[j] == '\t')
			j++;
		if (input_copy[j] == '\0')
			break;

		/* Handle operators */
		if (input_copy[j] == '|' || input_copy[j] == '>' || input_copy[j] == '<' || input_copy[j] == ';' || input_copy[j] == '&')
		{
			/* Check for logical operators (|| and &&) */
			if ((input_copy[j] == '|' && input_copy[j + 1] == '|') ||
				(input_copy[j] == '&' && input_copy[j + 1] == '&'))
			{
				/* Create a separate token for logical operators */
				char op[3];
				op[0] = input_copy[j];
				op[1] = input_copy[j];
				op[2] = '\0';

				/* Set up the operator token */
				commands[i] = malloc(sizeof(char *) * 2);
				if (!commands[i])
				{
					free_tokens(commands);
					free(input_copy);
					return (NULL);
				}

				commands[i][0] = _strdup(op);
				if (!commands[i][0])
				{
					free(commands[i]);
					free_tokens(commands);
					free(input_copy);
					return (NULL);
				}
				commands[i][1] = NULL;

				i++;
				j += 2; /* Skip both characters of the logical operator */
				continue;
			}
			else
			{
				is_double = (input_copy[j] == '>' || input_copy[j] == '<') &&
					input_copy[j + 1] == input_copy[j];
			}

			/* Special handling for heredoc (<<) */
			if (input_copy[j] == '<' && input_copy[j + 1] == '<')
			{
				/* Create the << operator token */
				if (!handle_operator(commands, i, input_copy[j], 1))
				{
					free_tokens(commands);
					free(input_copy);
					return (NULL);
				}
				i++;
				j += 2;

				/* Skip any whitespace */
				while (input_copy[j] == ' ' || input_copy[j] == '\t')
					j++;

				/* Extract the delimiter */
				start = j;
				while (input_copy[j] != '\0' && input_copy[j] != ' ' &&
					input_copy[j] != '\t' && input_copy[j] != '\n' &&
					input_copy[j] != '|' && input_copy[j] != '>' &&
					input_copy[j] != '<')
					j++;

				/* Create a token for the delimiter */
				if (j > start)
				{
					tmp = input_copy[j];
					input_copy[j] = '\0';

					/* Allocate memory for delimiter array */
					commands[i] = malloc(sizeof(char *) * 2);
					if (!commands[i])
					{
						free_tokens(commands);
						free(input_copy);
						return (NULL);
					}

					/* Copy delimiter string */
					commands[i][0] = _strdup(&input_copy[start]);
					if (!commands[i][0])
					{
						free(commands[i]);
						free_tokens(commands);
						free(input_copy);
						return (NULL);
					}
					commands[i][1] = NULL;

					/* Restore original character */
					input_copy[j] = tmp;
					i++;
				}
			}
			else
			{
				/* Standard operator handling */
				if (!handle_operator(commands, i, input_copy[j], is_double))
				{
					free_tokens(commands);
					free(input_copy);
					return (NULL);
				}
				i++;
				j += is_double ? 2 : 1;
			}
		}
		/* Handle regular commands */
		else
		{
			start = j;
			/* Find the end of the current command (until next operator or end of string) */
			while (input_copy[j] != '\0' &&
				   input_copy[j] != '|' &&
				   input_copy[j] != '>' &&
				   input_copy[j] != '<' &&
				   input_copy[j] != ';' &&
				   input_copy[j] != '&')
				j++;

			/* Temporarily null-terminate the command string */
			tmp = input_copy[j];
			input_copy[j] = '\0';

			/* Process current command segment with tokenize_args */
			commands[i] = tokenize_args(&input_copy[start]);
			if (!commands[i])
			{
				/* Clean up on failure */
				free_tokens(commands);
				free(input_copy);
				return (NULL);
			}
			i++;
			/* Restore the original character */
			input_copy[j] = tmp;
		}
	}
	/* Set NULL terminator for commands array */
	commands[i] = NULL;
	free(input_copy);
	return (commands);
}
