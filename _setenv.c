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

	new_var = (char *)malloc(_strlen(name) + _strlen(value) + 2);
	if (new_var == NULL)
	{
		perror("_setenv");
		return (-1);
	}

	_strcpy(new_var, name);
	_strcat(new_var, "=");
	_strcat(new_var, value);

	result = _putenv(new_var);
	free(new_var);
	if (result != 0)
	{
		perror("_setenv");
		return (-1);
	}
	return (0);
}
