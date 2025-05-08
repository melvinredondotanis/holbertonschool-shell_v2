#include "hsh.h"

/**
 * main - Test program for prompt functions
 *
 * This function demonstrates how to use the prompt functions.
 *
 * Return: 0 on success
 */
int main(void)
{
	prompt_t *prmt;
	int status;

	prmt = init_prompt();
	if (!prmt)
	{
		fprintf(stderr, "Failed to initialize prompt\n");
		return (1);
	}

	printf("Enter commands (Ctrl+D to exit):\n");

	while (1)
	{
		status = get_prompt(prmt);
		if (status == -1)
		{
			printf("\nExiting...\n");
			break;
		}

		printf("You entered: '%s' (length: %u)\n",
			   prmt->input, prmt->size);
		prmt->line_count++;
		printf("Line count: %d\n", prmt->line_count);
	}

	prmt = free_prompt(prmt);
	return (0);
}
