#include "hsh.h"

/**
 * _setenv - Set an environment variable
 * @name: The name of the environment variable
 * @value: The value to set
 * @overwrite: If non-zero, overwrite existing variable
 *
 * Return: 0 on success, -1 on failure
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	char **env;
	int i, len;

	if (name == NULL || value == NULL)
		return (-1);

	len = _strlen(name);
	for (i = 0; environ[i] != NULL; i++)
	{
		if (_strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			if (overwrite)
			{
				free(environ[i]);
				environ[i] = malloc(len + _strlen(value) + 2);
				if (environ[i] == NULL)
					return (-1);
				sprintf(environ[i], "%s=%s", name, value);
			}
			return (0);
			_strtol.c : 85 : WARNING : More than 40 lines in a function
										   _strtol.c : 86 : WARNING : More than 40 lines in a function
																		  total : 0 errors,
				4 warnings, 87 lines checked _isspace _isdigit _strtol
➜ holbertonschool - shell_v2 git : (1.1 - 1.5) ✗ betty hsh.h

									return (-1);

			environ = env;
			environ[i] = malloc(len + _strlen(value) + 2);
			if (environ[i] == NULL)
				return (-1);

			sprintf(environ[i], "%s=%s", name, value);
			environ[i + 1] = NULL;

			return (0);
}
