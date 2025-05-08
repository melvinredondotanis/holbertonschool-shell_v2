#include "hsh.h"

/**
 * main - Test function for built-in commands
 *
 * Return: 0 on success
 */
int main(void)
{
	char *exit_args[] = {"exit", NULL};
	char *exit_with_status[] = {"exit", "42", NULL};
	char *exit_bad[] = {"exit", "abc", NULL};
	char *cd_home[] = {"cd", NULL};
	char *cd_dir[] = {"cd", "/tmp", NULL};
	char *cd_prev[] = {"cd", "-", NULL};
	char *env_args[] = {"env", NULL};
	int status = 0;
	int result;

	printf("===== Testing builtin commands =====\n\n");

	/* Test env command */
	printf("Testing env command (showing first 3 entries):\n");
	setenv("TEST_VAR", "test_value", 1);
	result = handle_builtin(env_args, &status);
	printf("Result: %d, Status: %d\n\n", result, status);

	/* Test cd command */
	printf("Testing cd to /tmp:\n");
	result = handle_builtin(cd_dir, &status);
	printf("Current directory: ");
	system("pwd");
	printf("Result: %d, Status: %d\n\n", result, status);

	printf("Testing cd to home:\n");
	result = handle_builtin(cd_home, &status);
	printf("Current directory: ");
	system("pwd");
	printf("Result: %d, Status: %d\n\n", result, status);

	printf("Testing cd to previous dir:\n");
	result = handle_builtin(cd_prev, &status);
	printf("Current directory: ");
	system("pwd");
	printf("Result: %d, Status: %d\n\n", result, status);

	/* Test exit command */
	printf("Testing exit with no args (should just print 'exit'):\n");
	result = handle_builtin(exit_args, &status);
	printf("Result: %d, Status: %d\n\n", result, status);

	printf("Testing exit with status 42:\n");
	result = handle_builtin(exit_with_status, &status);
	printf("Result: %d, Status: %d\n\n", result, status);

	printf("Testing exit with bad argument:\n");
	result = handle_builtin(exit_bad, &status);
	printf("Result: %d, Status: %d\n\n", result, status);

	return (0);
}
