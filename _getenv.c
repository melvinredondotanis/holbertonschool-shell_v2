#include "hsh.h"

/**
 * _getenv - Get the value of an environment variable
 * @name: Name of the environment variable
 *
 * Return: Pointer to the value of the environment variable, or NULL if not found
 */
char *_getenv(const char *name)
{
	char **env = NULL;
	size_t name_len = _strlen(name);

	for (env = environ; *env != NULL; env++)
		if (_strncmp(*env, name, name_len) == 0 && (*env)[name_len] == '=')
			return (&(*env)[name_len + 1]);
	return (NULL);
}
