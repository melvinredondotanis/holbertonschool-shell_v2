#include "hsh.h"

/**
 * builtin_exit - Handle the exit built-in command
 * @args: Arguments passed to the exit command
 * @status: Pointer to the current status (to be updated)
 * @program_name: The name of the shell program
 * @line_count: The current line number
 *
 * Return: 1 to indicate the shell should exit, 0 otherwise
 */
int builtin_exit(char **args, int *status, char *program_name, int line_count)
{
	int exit_status = *status;

	if (args[1])
	{
		char *endptr;
		long val = strtol(args[1], &endptr, 10);

		if (*endptr != '\0' || val < 0 || (val == 0 && strcmp(args[1], "0") != 0 && strcmp(args[1], "+0") != 0))
		{
			fprintf(stderr, "%s: %d: exit: %s: numeric argument required\n",
					program_name, line_count, args[1]);
			*status = 2;
			return (0);
		}
		exit_status = (int)(val % 256);
	}

	*status = exit_status;
	if (isatty(STDIN_FILENO))
		printf("exit\n");
	return (1);
}

/**
 * builtin_cd - Handle the cd built-in command
 * @args: Arguments passed to the cd command
 * @program_name: The name of the shell program
 * @line_count: The current line number
 *
 * Return: 0 on success, -1 on failure
 */
int builtin_cd(char **args, char *program_name, int line_count)
{
	char cwd[PATH_MAX];
	char *dir = NULL, *oldpwd = NULL, *target_arg = NULL;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}

	if (!args[1] || strcmp(args[1], "~") == 0)
	{
		dir = getenv("HOME");
		if (!dir)
		{
			fprintf(stderr, "%s: %d: cd: HOME not set\n", program_name, line_count);
			return (-1);
		}
	}
	else if (strcmp(args[1], "-") == 0)
	{
		dir = getenv("OLDPWD");
		if (!dir)
		{
			fprintf(stderr, "%s: %d: cd: OLDPWD not set\n", program_name, line_count);
			return (-1);
		}
		printf("%s\n", dir);
		target_arg = "-";
	}
	else
	{
		dir = args[1];
		target_arg = args[1];
	}

	oldpwd = strdup(cwd);
	if (!oldpwd)
	{
		perror("strdup");
		return (-1);
	}

	if (chdir(dir) == -1)
	{
		char error_prefix[PATH_MAX + 100];

		snprintf(error_prefix, sizeof(error_prefix), "%s: %d: cd: %s",
				 program_name, line_count, target_arg ? target_arg : dir);
		perror(error_prefix);
		free(oldpwd);
		return (-1);
	}

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		free(oldpwd);
		return (-1);
	}

	if (setenv("OLDPWD", oldpwd, 1) == -1 || setenv("PWD", cwd, 1) == -1)
	{
		perror("setenv");
		free(oldpwd);
		return (-1);
	}

	free(oldpwd);
	return (0);
}

/**
 * builtin_env - Handle the env built-in command
 *
 * Return: Always returns 0
 */
int builtin_env(void)
{
	int i;

	for (i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);

	return (0);
}

/**
 * handle_builtin - Check if command is a built-in and execute it
 * @args: Command and its arguments
 * @status: Pointer to the shell status
 * @program_name: The name of the shell program
 * @line_count: The current line number
 *
 * Return: 1 if a built-in was executed, 0 otherwise, -1 on exit
 */
int handle_builtin(char **args, int *status, char *program_name, int line_count)
{
	if (!args || !args[0])
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		if (builtin_exit(args, status, program_name, line_count))
			return (-1);
		return (1);
	}
	else if (strcmp(args[0], "cd") == 0)
	{
		*status = builtin_cd(args, program_name, line_count) == 0 ? 0 : 1;
		return (1);
	}
	else if (strcmp(args[0], "env") == 0)
	{
		*status = builtin_env();
		return (1);
	}

	return (0);
}
