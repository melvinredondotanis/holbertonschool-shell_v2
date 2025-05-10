#include "hsh.h"

/**
 * find_command_path - Find the full path of a command using PATH
 * @command: The command to find
 *
 * Return: The full path of the command, or NULL if not found
 *
 * Example:
 *   Input: find_command_path("ls")
 *   Output: "/bin/ls"
 */
static char *find_command_path(char *command)
{
	char *path_env, *path_copy, *path_token, *file_path;
	int command_len, dir_len;
	struct stat buffer;

	/* Validate command input */
	if (!command || !*command)
		return (NULL);

	/* If command contains '/', treat it as a path */
	if (_strchr(command, '/') != NULL)
	{
		if (stat(command, &buffer) == 0 && (buffer.st_mode & S_IXUSR))
			return (_strdup(command));
		return (NULL);
	}

	/* Get PATH environment variable */
	path_env = _getenv("PATH");
	if (!path_env)
		return (NULL);

	/* Create a copy to tokenize safely */
	path_copy = _strdup(path_env);
	if (!path_copy)
		return (NULL);

	/* Search in each directory in PATH */
	command_len = _strlen(command);
	path_token = strtok(path_copy, ":");
	while (path_token)
	{
		/* Allocate memory for the full path */
		dir_len = _strlen(path_token);
		file_path = malloc(dir_len + command_len + 2); /* +2 for '/' and '\0' */
		if (!file_path)
		{
			free(path_copy);
			return (NULL);
		}

		/* Build the full path */
		_strcpy(file_path, path_token);
		_strcat(file_path, "/");
		_strcat(file_path, command);

		/* Check if file exists and is executable */
		if (stat(file_path, &buffer) == 0 && (buffer.st_mode & S_IXUSR))
		{
			free(path_copy);
			return (file_path);
		}

		/* Try next directory */
		free(file_path);
		path_token = strtok(NULL, ":");
	}

	/* Command not found in any PATH directory */
	free(path_copy);
	return (NULL);
}

/**
 * handle_heredoc - Handle heredoc redirection
 * @delimiter: The delimiter string that ends the heredoc
 * @pipefd: Pipe file descriptors for communication
 *
 * Return: 0 on success, -1 on failure
 */
static int handle_heredoc(char *delimiter, int pipefd[2])
{
	char *line = NULL;
	size_t len = 0;
	ssize_t read_len;
	int is_interactive = isatty(STDIN_FILENO);

	/* Display prompt if in interactive mode */
	if (is_interactive)
		write(STDOUT_FILENO, "> ", 2);

	while ((read_len = getline(&line, &len, stdin)) != -1)
	{
		/* Remove newline if present */
		if (read_len > 0 && line[read_len - 1] == '\n')
			line[read_len - 1] = '\0';

		/* Check if line matches delimiter exactly */
		if (strcmp(line, delimiter) == 0)
			break;

		/* Write the line to the pipe with a newline */
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);

		/* Display prompt for next line if in interactive mode */
		if (is_interactive)
			write(STDOUT_FILENO, "> ", 2);
	}

	/* Clean up */
	free(line);
	return (0);
}

/**
 * setup_redirections - Configure input and output redirections
 * @tokens: The array of tokens
 * @idx: Current position in the token array
 * @saved_fds: Array to save original stdin (0) and stdout (1)
 * @program_name: Name of the shell program
 * @line_count: Current line count for error messages
 *
 * Return: Index of the next command, or -1 on error
 */
static int setup_redirections(char ***tokens, int idx, int saved_fds[2], char *program_name, int line_count)
{
	int fd, i = idx;

	/* Initialize saved file descriptors as -1 (unset) */
	saved_fds[0] = -1;
	saved_fds[1] = -1;

	while (tokens[i] != NULL)
	{
		/* Skip non-redirection tokens */
		if (!tokens[i][0] || !tokens[i][0][0] ||
			(tokens[i][0][0] != '<' && tokens[i][0][0] != '>' && tokens[i][0][0] != '|'))
			{
				i++;
				continue;
			}

		/* Handle redirections */
		if (tokens[i][0][0] == '<' || tokens[i][0][0] == '>')
		{
			int is_input = (tokens[i][0][0] == '<');
			int is_append = (_strcmp(tokens[i][0], ">>") == 0);
			int is_heredoc = (_strcmp(tokens[i][0], "<<") == 0);
			int target_fd = is_input ? STDIN_FILENO : STDOUT_FILENO;
			int flags;

			/* Check if filename/delimiter exists */
			if (!tokens[i + 1] || !tokens[i + 1][0])
				return (-1);

			/* Handle heredoc (<<) */
			if (is_heredoc)
			{
				char *delimiter = tokens[i + 1][0];
				int pipefd[2];

				/* Create a pipe */
				if (pipe(pipefd) == -1)
				{
					perror("pipe");
					return (-1);
				}

				/* Save original stdin */
				saved_fds[0] = dup(STDIN_FILENO);
				if (saved_fds[0] == -1)
				{
					close(pipefd[0]);
					close(pipefd[1]);
					return (-1);
				}

				/* Handle heredoc input */
				if (handle_heredoc(delimiter, pipefd) == -1)
				{
					close(pipefd[0]);
					close(pipefd[1]);
					return (-1);
				}

				/* Connect pipe to stdin */
				dup2(pipefd[0], STDIN_FILENO);
				close(pipefd[0]);
				close(pipefd[1]);

				i += 2;
			}
			else
			{
				/* Set file open flags */
				if (is_input)
					flags = O_RDONLY;
				else
					flags = O_WRONLY | O_CREAT | (is_append ? O_APPEND : O_TRUNC);

				/* Open file */
				fd = open(tokens[i + 1][0], flags, 0644);
				if (fd == -1)
				{
					/* Print more specific error message */
					if (is_input && access(tokens[i + 1][0], F_OK) == -1)
						fprintf(stderr, "%s: %d: %s: No such file or directory\n", program_name, line_count, tokens[i + 1][0]);
					else if (is_input)
						fprintf(stderr, "%s: %d: %s: Permission denied\n", program_name, line_count, tokens[i + 1][0]);
					else
						fprintf(stderr, "%s: %d: %s: Permission denied\n", program_name, line_count, tokens[i + 1][0]);
					return (-1);
				}

				/* Save original fd and redirect */
				saved_fds[is_input ? 0 : 1] = dup(target_fd);
				if (saved_fds[is_input ? 0 : 1] == -1)
				{
					close(fd);
					return (-1);
				}
				dup2(fd, target_fd);
				close(fd);
				i += 2;
			}
		}
		/* Return index when pipe is found */
		else if (tokens[i][0][0] == '|')
			return (i);
	}
	return (i);
}

/**
 * execute_command - Execute commands with or without pipeline
 * @tokens: The array of tokenized commands
 * @program_name: Name of the shell program
 * @line_count: Current line count for error messages
 *
 * Return: Exit status of the last command
 */
static int execute_command(char ***tokens, char *program_name, int line_count)
{
	int i = 0, status = 0;
	int pipe_fds[2][2], curr_pipe = 0;
	int saved_fds[2] = {-1, -1};
	pid_t pid;

	/* Check for builtin exit command at the beginning to avoid unnecessary fork */
	if (tokens[0] && tokens[0][0])
	{
		/* First check if it's the exit command to handle it specially */
		if (_strcmp(tokens[0][0], "exit") == 0)
		{
			int exit_result = handle_builtin(tokens[0], &status, program_name, line_count);
			if (exit_result == -1)
				return (status); /* Return with the exit status */
		}

		/* Then check if it's any other builtin command without pipes or redirections */
		int j, has_pipe = 0, has_redir = 0;
		for (j = 0; tokens[j] != NULL; j++)
		{
			if (tokens[j][0])
			{
				if (_strcmp(tokens[j][0], "|") == 0)
					has_pipe = 1;
				else if (_strchr("><", tokens[j][0][0]) != NULL)
					has_redir = 1;
			}
		}

		/* Execute builtin directly if no pipes or redirections are present */
		if (!has_pipe && !has_redir)
		{
			int builtin_result = handle_builtin(tokens[0], &status, program_name, line_count);
			if (builtin_result == 1)
				return (status); /* Builtin executed successfully */
			else if (builtin_result == -1)
				return (status); /* Exit requested */
		}
	}

	while (tokens[i] != NULL)
	{
		/* Skip empty commands and operators */
		if (!tokens[i][0] || !tokens[i][0][0] ||
			_strchr("<>|;", tokens[i][0][0]))
		{
			i++;
			continue;
		}

		 /* Handle builtin commands without forking if not in a pipeline */
		int j, has_pipe = 0;
		for (j = 0; tokens[j] != NULL; j++)
		{
			if (tokens[j][0] && _strcmp(tokens[j][0], "|") == 0)
			{
				has_pipe = 1;
				break;
			}
		}

		if (!has_pipe && tokens[i][0])
		{
			/* Try executing builtin directly */
			int builtin_result = handle_builtin(tokens[i], &status, program_name, line_count);
			if (builtin_result == 1)
			{
				/* Builtin executed successfully */
				return (status);
			}
			else if (builtin_result == -1)
			{
				/* Exit requested */
				exit(status);
			}
		}

		/* Check for pipe after current command */
		int has_next_pipe = 0;
		for (j = i + 1; tokens[j] != NULL; j++)
		{
			if (tokens[j][0] && _strcmp(tokens[j][0], "|") == 0)
			{
				has_next_pipe = 1;
				if (pipe(pipe_fds[curr_pipe]) == -1)
				{
					perror("pipe");
					return (1);
				}
				break;
			}
			if (tokens[j][0] && !_strchr("<>", tokens[j][0][0]) && tokens[j][0][0] != '\0')
				break;
		}

		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}

		if (pid == 0)
		{
			/* Child process */
			char *command_path;

			/* Set up pipe I/O */
			if (curr_pipe == 1)
			{
				dup2(pipe_fds[0][0], STDIN_FILENO);
				close(pipe_fds[0][0]);
				close(pipe_fds[0][1]);
			}

			if (has_next_pipe)
			{
				dup2(pipe_fds[curr_pipe][1], STDOUT_FILENO);
				close(pipe_fds[curr_pipe][0]);
				close(pipe_fds[curr_pipe][1]);
			}

			/* Set up redirections - must be done before trying to read input in builtin or exec */
			if (setup_redirections(tokens, i + 1, saved_fds, program_name, line_count) == -1)
				exit(1);

			/* Try builtin commands */
			if (handle_builtin(tokens[i], &status, program_name, line_count) != 0)
				exit(status);

			/* Execute external command */
			command_path = find_command_path(tokens[i][0]);
			if (!command_path)
			{
				fprintf(stderr, "%s: %d: %s: not found\n", program_name, line_count, tokens[i][0]);
				/* No need to free NULL pointer */
				free_tokens(tokens);
				exit(127);
			}
			execve(command_path, tokens[i], environ);
			perror("execve");
			free(command_path);
			free_tokens(tokens); /* Free tokens before exit */
			exit(1);
		}
		else
		{
			/* Parent process */
			/* Close previous pipe */
			if (curr_pipe == 1)
			{
				close(pipe_fds[0][0]);
				close(pipe_fds[0][1]);
			}

			if (has_next_pipe)
				curr_pipe = 1 - curr_pipe;

			/* Find next command */
			int next_cmd = i + 1;

			while (tokens[next_cmd] != NULL)
			{
				if (tokens[next_cmd][0] && _strcmp(tokens[next_cmd][0], "|") == 0)
				{
					next_cmd++;
					break;
				}
				next_cmd++;
			}

			i = (tokens[next_cmd] != NULL) ? next_cmd : -1;
			if (i < 0)
				break;
		}
	}

	/* Wait for child processes */
	while (wait(&status) > 0)
		;

	/* Restore redirections */
	for (int j = 0; j < 2; j++)
		if (saved_fds[j] != -1)
		{
			dup2(saved_fds[j], j);
			close(saved_fds[j]);
			saved_fds[j] = -1;
		}

	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

/**
 * interpret_tokens - Interpret tokenized commands
 * @tokens: The array of tokenized commands
 * @program_name: Name of the shell program
 * @line_count: Current line count for error messages
 *
 * Return: Exit status of the command
 */
int interpret_tokens(char ***tokens, char *program_name, int line_count)
{
	int i = 0, j, status = 0, semicolon_found;
	char ***cmd_segment;

	if (!tokens || !tokens[0] || !tokens[0][0])
		return (0);

	/* Check if there are any semicolons in the command */
	semicolon_found = 0;
	for (j = 0; tokens[j] != NULL; j++)
	{
		if (tokens[j][0] && _strcmp(tokens[j][0], ";") == 0)
		{
			semicolon_found = 1;
			break;
		}
	}

	/* If no semicolons, just execute the command as is */
	if (!semicolon_found)
		return (execute_command(tokens, program_name, line_count));

	/* Handle multiple commands separated by semicolons */
	while (tokens[i] != NULL)
	{
		j = i;
		/* Find the next semicolon or the end of tokens */
		while (tokens[j] != NULL &&
			  (tokens[j][0] == NULL || _strcmp(tokens[j][0], ";") != 0))
			j++;

		/* Create a temporary array for the current command segment */
		cmd_segment = malloc(sizeof(char **) * (j - i + 1));
		if (!cmd_segment)
			return (1);

		/* Copy tokens for the current command segment */
		int k;
		for (k = 0; i + k < j; k++)
			cmd_segment[k] = tokens[i + k];
		cmd_segment[k] = NULL;

		/* Execute the current command segment */
		if (cmd_segment[0] != NULL && cmd_segment[0][0] != NULL)
			status = execute_command(cmd_segment, program_name, line_count);

		/* Free the temporary array (but not its contents) */
		free(cmd_segment);

		/* Skip the semicolon token */
		i = j;
		if (tokens[i] != NULL)
			i++;
	}

	return (status);
}
