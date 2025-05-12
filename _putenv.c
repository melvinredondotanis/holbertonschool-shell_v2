#include "hsh.h"

/* Global variables to track allocations */
static char **allocated_vars = ((void *)0);
static int alloc_count = 0;
static char **old_environ = ((void *)0);

/**
 * _putenv - Set or update an environment variable
 * @string: Name of the environment variable.
 *
 * Return: 0 on success, -1 on failure
 */
int _putenv(char *string)
{
	extern char **environ;
	int i, j;
	char *new_var, *name, *value;
	size_t name_len;

	/* Use custom tokenization instead of strtok */
	name = string;
	value = NULL;
	for (i = 0; string[i] != '\0'; i++)
	{
		if (string[i] == '=')
		{
			string[i] = '\0'; /* Temporarily replace '=' with null terminator */
			value = &string[i + 1];
			break;
		}
	}

	if (!name || !value)
		return (-1);

	name_len = _strlen(name);

	/* Create the new environment variable string */
	new_var = malloc(name_len + _strlen(value) + 2); /* +2 for '=' and null terminator */
	if (!new_var)
		return (-1);

	_strcpy(new_var, name);
	_strcat(new_var, "=");
	_strcat(new_var, value);

	/* Restore the '=' in the original string */
	if (value)
		string[i] = '=';

	/* Track this new allocation */
	char **temp = _realloc(allocated_vars, sizeof(char *) * (alloc_count + 1));
	if (!temp)
	{
		free(new_var);
		return (-1);
	}
	allocated_vars = temp;
	allocated_vars[alloc_count++] = new_var;

	/* Look for an existing variable with this name */
	for (i = 0; environ[i]; i++)
	{
		if (_strncmp(environ[i], name, name_len) == 0 && environ[i][name_len] == '=')
		{
			/* Found it, replace it */
			environ[i] = new_var;
			return (0);
		}
	}

	/* Variable not found, add it to the end */
	/* First, make a copy of the environment */
	char **new_environ = malloc((i + 2) * sizeof(char *));
	if (!new_environ)
	{
		free(new_var);
		return (-1);
	}

	/* Copy existing environment */
	for (j = 0; j < i; j++)
		new_environ[j] = environ[j];

	/* Add new variable and NULL terminator */
	new_environ[i] = new_var;
	new_environ[i + 1] = NULL;

	/* Keep track of this environment allocation */
	if (old_environ != NULL && old_environ != environ)
		free(old_environ);

	old_environ = new_environ;
	environ = new_environ;

	return (0);
}

/**
 * cleanup_environment - Free all allocated environment memory
 * This function should be called before program exit
 */
void cleanup_environment(void)
{
	int i;

	/* Free all allocated environment variable strings */
	if (allocated_vars)
	{
		for (i = 0; i < alloc_count; i++)
		{
			if (allocated_vars[i])
				free(allocated_vars[i]);
		}
		free(allocated_vars);
		allocated_vars = NULL;
		alloc_count = 0;
	}

	/* Free the environment array itself if we allocated it */
	if (old_environ != NULL)
	{
		free(old_environ);
		old_environ = NULL;
	}
}
