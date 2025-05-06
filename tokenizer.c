#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * free_tokens - Frees the memory allocated for the tokens
 * @tokens: The array of tokens to free
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
 * Return: A pointer to an array of strings, or NULL on failure
 */
static char **tokenize_args(char *command)
{
	char **args;
	char *token, *str;
	char *saveptr;
	int i, j;

	args = malloc(sizeof(char *) * 64);
	str = strdup(command);
	i = 0;

	if (!args || !str)
		return (NULL);

	token = strtok_r(str, " ", &saveptr);
	while (token != NULL)
	{
		args[i] = strdup(token);
		if (!args[i])
		{
			free(str);
			for (j = 0; j < i; j++)
				free(args[j]);
			free(args);
			return (NULL);
		}
		i++;
		token = strtok_r(NULL, " ", &saveptr);
	}
	args[i] = NULL;
	free(str);
	return (args);
}

/**
 * tokenize_command - Tokenizes a command string
 * @input: The input string to tokenize
 *
 * Return: A pointer to an array of strings, or NULL on failure
 */
char ***tokenize_command(char *input)
{
	char *input_copy;
	int i, j, start;
	char ***commands;
	char tmp;
	char op[3];

	input_copy = strdup(input);
	i = 0;
	j = 0;
	commands = malloc(sizeof(char **) * 64); /* Initial size */

	if (!commands || !input_copy)
		return (NULL);

	while (input_copy[j] != '\0')
	{
		/* Skip leading spaces */
		while (input_copy[j] == ' ')
			j++;

		if (input_copy[j] == '\0')
			break;

		/* Determine if current character is the start of an operator */
		if (input_copy[j] == '|' || input_copy[j] == '>' || input_copy[j] == '<')
		{
			/* Handle double operators (>> or <<) */
			if ((input_copy[j] == '>' || input_copy[j] == '<') &&
				input_copy[j + 1] == input_copy[j])
			{
				commands[i] = malloc(sizeof(char *) * 2);
				if (!commands[i])
				{
					free_tokens(commands);
					free(input_copy);
					return (NULL);
				}
				op[0] = input_copy[j];
				op[1] = input_copy[j + 1];
				op[2] = '\0';
				commands[i][0] = strdup(op);
				commands[i][1] = NULL;
				j += 2;
			}
			else
			{
				commands[i] = malloc(sizeof(char *) * 2);
				if (!commands[i])
				{
					free_tokens(commands);
					free(input_copy);
					return (NULL);
				}
				op[0] = input_copy[j];
				op[1] = '\0';
				commands[i][0] = strdup(op);
				commands[i][1] = NULL;
				j++;
			}
			i++;
		}
		else
		{
			/* This is the start of a command */
			start = j;

			/* Find end of command (until next operator or end of string) */
			while (input_copy[j] != '\0' &&
				   input_copy[j] != '|' &&
				   input_copy[j] != '>' &&
				   input_copy[j] != '<')
			{
				j++;
			}

			/* Null-terminate the command temporarily */
			tmp = input_copy[j];
			input_copy[j] = '\0';

			/* Parse the command and its arguments */
			commands[i] = tokenize_args(&input_copy[start]);
			if (!commands[i])
			{
				free_tokens(commands);
				free(input_copy);
				return (NULL);
			}
			i++;

			/* Restore the character */
			input_copy[j] = tmp;
		}
	}

	commands[i] = NULL; /* Null-terminate the command list */
	free(input_copy);
	return (commands);
}
