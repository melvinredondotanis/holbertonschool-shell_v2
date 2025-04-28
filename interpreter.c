#include "hsh.h"

/**
 * parse_command - Parse a command line into command and arguments
 * @input: The input string to parse
 *
 * Return: An array of strings containing the command and its arguments
 */
char **parse_command(char *input)
{
	char **args = NULL;
	char *token;
	int i = 0, capacity = 10;

	if (!input || !*input)
		return (NULL);

	args = malloc(sizeof(char *) * capacity);
	check_alloc(args);

	for (token = strtok(input, " \t\n"); token; token = strtok(NULL, " \t\n"))
	{
		if (i >= capacity - 1)
		{
			capacity *= 2;
			args = realloc(args, sizeof(char *) * capacity);
			check_alloc(args);
		}
		args[i] = strdup(token);
		check_alloc(args[i++]);
	}
	args[i] = NULL;
	return (args);
}

/**
 * find_command_path - Find the full path of a command using PATH
 * @command: The command to find
 *
 * Return: The full path of the command, or NULL if not found
 */
char *find_command_path(char *command)
{
	char *path_env, *path_copy, *path_token, *file_path;
	int command_len, dir_len;
	struct stat buffer;

	if (!command || !*command)
		return (NULL);

	if (strchr(command, '/') != NULL)
		return (stat(command, &buffer) == 0 && (buffer.st_mode & S_IXUSR) ?
				strdup(command) : NULL);

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	check_alloc(path_copy);
	command_len = strlen(command);

	for (path_token = strtok(path_copy, ":"); path_token;
	path_token = strtok(NULL, ":"))
	{
		dir_len = strlen(path_token);
		file_path = malloc(dir_len + command_len + 2);
		check_alloc(file_path);

		sprintf(file_path, "%s/%s", path_token, command);

		if (stat(file_path, &buffer) == 0 && (buffer.st_mode & S_IXUSR))
		{
			free(path_copy);
			return (file_path);
		}
		free(file_path);
	}
	free(path_copy);
	return (NULL);
}

/**
 * execute_command - Execute a command with its arguments
 * @command_path: The full path of the command to execute
 * @args: The arguments for the command
 *
 * Return: The exit status of the command, or -1 on error
 */
int execute_command(char *command_path, char **args)
{
	pid_t pid;
	int status;

	if (!command_path || !args)
		return (-1);

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		if (execve(command_path, args, environ) == -1)
		{
			perror(args[0]);
			exit(EXIT_FAILURE);
		}
	}
	else if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (-1);
	}

	return (WIFEXITED(status) ? WEXITSTATUS(status) : 0);
}

/**
 * free_args - Free the memory allocated for arguments
 * @args: The array of arguments to free
 */
void free_args(char **args)
{
	int i;

	if (!args)
		return;

	for (i = 0; args[i]; i++)
		free(args[i]);
	free(args);
}

/**
 * interpret_command - Interpret and execute a command
 * @prompt: The prompt structure containing the command
 *
 * Return: The exit status of the command, or -1 on error
 */
int interpret_command(prompt_t *prompt)
{
	char **args, *command_path;
	int status = 0;

	if (!prompt || !prompt->input || !*(prompt->input))
		return (0);

	args = parse_command(prompt->input);
	if (!args || !args[0])
	{
		free_args(args);
		return (0);
	}

	command_path = find_command_path(args[0]);
	if (!command_path)
	{
		fprintf(stderr, "%s: command not found\n", args[0]);
		free_args(args);
		return (127);
	}

	status = execute_command(command_path, args);
	free(command_path);
	free_args(args);
	return (status);
}
