#include "hsh.h"

/**
 * builtin_exit - Handle the exit built-in command
 * @args: Arguments passed to the exit command
 * @status: Pointer to the current status (to be updated)
 * @program_name: Name of the shell program
 * @line_count: Current line count for error messages
 *
 * Examples:
 *   exit       - Exits with current status
 *   exit 42    - Exits with status 42
 *   exit abc   - Prints error and sets status to 2
 *
 * Return: -1 to indicate the shell should exit, 0 otherwise
 */
static int builtin_exit(char **args, int *status, char *program_name, int line_count)
{
	int exit_status = *status;
	long val;
	char *endptr;

	if (args[1])
	{
		val = _strtol(args[1], &endptr, 10);

		if (*endptr != '\0' || val < 0)
		{
			fprintf(stderr, "%s: %d: exit: Illegal number: %s\n",
					program_name, line_count, args[1]);
			*status = 2;
			return (0);
		}
		exit_status = (int)(val % 256);
	}

	*status = exit_status;
	if (isatty(STDIN_FILENO))
		printf("exit\n");

	return (-1);
}

/**
 * builtin_cd - Handle the cd built-in command
 * @args: Arguments passed to the cd command
 * @program_name: Name of the shell program
 * @line_count: Current line count for error messages
 *
 * Examples:
 *   cd         - Changes to HOME directory
 *   cd ~       - Changes to HOME directory
 *   cd -       - Changes to previous directory (OLDPWD)
 *   cd /path   - Changes to specified path
 *
 * Return: 0 on success, -1 on failure
 */
static int builtin_cd(char **args, char *program_name, int line_count)
{
	char cwd[PATH_MAX];
	char *dir = NULL;
	char *oldpwd = NULL;

	/* Get current working directory */
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		return (-1);
	}

	/* Determine target directory */
	if (!args[1] || _strcmp(args[1], "~") == 0)
	{
		dir = _getenv("HOME");
		if (!dir || dir[0] == '\0')
		{
			fprintf(stderr, "%s: %d: cd: HOME not set\n",
					program_name, line_count);
			return (-1);
		}
	}
	else if (_strcmp(args[1], "-") == 0)
	{
		dir = _getenv("OLDPWD");
		if (!dir || dir[0] == '\0')
		{
			fprintf(stderr, "%s: %d: cd: OLDPWD not set\n",
					program_name, line_count);
			return (-1);
		}
		printf("%s\n", dir);
	}
	else
		dir = args[1];

	/* Save old path to set OLDPWD later */
	oldpwd = _strdup(cwd);
	if (!oldpwd)
	{
		perror("_strdup");
		return (-1);
	}

	/* Change directory */
	if (chdir(dir) == -1)
	{
		fprintf(stderr, "%s: %d: cd: can't cd to %s\n",
				program_name, line_count, dir);
		free(oldpwd);
		return (-1);
	}

	/* Get new current directory */
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("getcwd");
		free(oldpwd);
		return (-1);
	}

	/* Update environment variables */
	if (_setenv("OLDPWD", oldpwd) == -1)
	{
		perror("_setenv");
		free(oldpwd);
		return (-1);
	}
	free(oldpwd);

	if (_setenv("PWD", cwd) == -1)
	{
		perror("_setenv");
		return (-1);
	}

	return (0);
}

/**
 * builtin_env - Handle the env built-in command
 *
 * Example:
 *   env        - Lists all environment variables
 *
 * Return: Always returns 0
 */
static int builtin_env(void)
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
 * @program_name: Name of the shell program
 * @line_count: Current line count for error messages
 *
 * Examples:
 *   args={"exit", NULL}                - Exits shell
 *   args={"cd", "/tmp", NULL}          - Changes directory
 *   args={"env", NULL}                 - Shows environment
 *
 * Return: 1 if a built-in was executed, 0 otherwise, -1 on exit
 */
int handle_builtin(char **args, int *status, char *program_name, int line_count)
{
	if (!args || !args[0])
		return (0);

	if (_strcmp(args[0], "exit") == 0)
	{
		if (builtin_exit(args, status, program_name, line_count) == -1)
			return (-1);
		return (1);
	}
	else if (_strcmp(args[0], "cd") == 0)
	{
		*status = builtin_cd(args, program_name, line_count) == 0 ? 0 : 1;
		return (1);
	}
	else if (_strcmp(args[0], "env") == 0)
	{
		*status = builtin_env();
		return (1);
	}
	else if (_strcmp(args[0], "setenv") == 0)
	{
		*status = builtin_setenv(args, program_name, line_count) == 0 ? 0 : 1;
		return (1);
	}
	else if (_strcmp(args[0], "unsetenv") == 0)
	{
		*status = builtin_unsetenv(args, program_name, line_count) == 0 ? 0 : 1;
		return (1);
	}

	return (0);
}
