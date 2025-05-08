#include "hsh.h"

/**
 * main - Test function for the tokenizer
 *
 * Return: Always 0
 */
int main(void)
{
	char ***tokens;
	int i, j;
	char *test_inputs[] = {
		"ls -l",
		"echo hello world",
		"ls -l | grep .c",
		"cat file > output.txt",
		"cat < input.txt > output.txt",
		NULL};

	for (i = 0; test_inputs[i] != NULL; i++)
	{
		printf("Input: '%s'\n", test_inputs[i]);
		tokens = tokenize_command(test_inputs[i]);

		if (!tokens)
		{
			printf("Error tokenizing command\n");
			continue;
		}

		printf("Tokens:\n");
		for (j = 0; tokens[j] != NULL; j++)
		{
			int k;

			printf("  Group %d: ", j);
			for (k = 0; tokens[j][k] != NULL; k++)
				printf("'%s' ", tokens[j][k]);
			printf("\n");
		}
		printf("\n");

		free_tokens(tokens);
	}

	return (0);
}
