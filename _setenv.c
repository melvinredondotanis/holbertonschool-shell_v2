#include "hsh.h"

/**
 * _setenv - Set or update an environment variable
 * @name: Name of the environment variable.
 * @value: Value to set for the environment variable
 *
 * Return: 0 on success, -1 on failure
 */
int _setenv(const char *name, const char *value)
{
	char *new_var = NULL;
	int result = 0;

	if (_getenv(name) != NULL)
	{
		new_var = (char *)malloc(strlen(name) + strlen(value) + 2);
		if (new_var == NULL)
		{
			perror("_setenv");
			return (-1);
		}
		sprintf(new_var, "%s=%s", name, value);
		result = putenv(new_var);
		if (result != 0)
		{
			perror("_setenv");
			free(new_var);
			return (-1);
		}
	}
	else
	{
		new_var = (char *)malloc(strlen(name) + strlen(value) + 2);
		if (new_var == NULL)
		{
			perror("_setenv");
			return (-1);
		}
		sprintf(new_var, "%s=%s", name, value);
		if (putenv(new_var) != 0)
		{
			perror("_setenv");
			free(new_var);
			return (-1);
		}
	}
	return (0);
}
