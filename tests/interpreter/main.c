#include "hsh.h"

/**
 * main - Test the interpreter
 * Return: Always 0
 */
int main(void)
{
	prompt_t *prompt = init_prompt();
	char *program_name = "hsh";
	int exit_req = 0;

	if (!prompt)
		return (EXIT_FAILURE);

	while (get_prompt(prompt) != -1)
	{
		char ***tokens = tokenize_command(prompt->input);
		if (!tokens)
			break;

		if (interpret_tokens(tokens, program_name) == -1 || exit_req)
			break;
	}

	free_prompt(prompt);
	return (EXIT_SUCCESS);
}
