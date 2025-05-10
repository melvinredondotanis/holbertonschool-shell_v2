#include "hsh.h"

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
			string[i] = '\0';  /* Temporarily replace '=' with null terminator */
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

	strcpy(new_var, name);
	_strcat(new_var, "=");
	_strcat(new_var, value);

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

	/* Avoid memory leaks by keeping track of allocated environment */
	static char **old_environ = ((void *) 0);
	static char **old_vars = ((void *) 0);
	static int old_var_count;

	if (old_environ != NULL && old_environ != environ)
	{
		free(old_environ);
		/* Also free any variables we've allocated previously that aren't in new_environ */
		for (j = 0; j < old_var_count; j++)
			if (old_vars[j] != NULL)
				free(old_vars[j]);
		free(old_vars);
		old_var_count = 0;
	}

	/* Keep track of allocated variables for later cleanup */
	old_vars = malloc(sizeof(char *));
	if (old_vars)
	{
		old_vars[0] = new_var;
		old_var_count = 1;
	}

	old_environ = new_environ;
	environ = new_environ;

	return (0);
}