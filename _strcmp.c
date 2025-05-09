#include "hsh.h"

/**
 * _strcmp - Compare two strings
 * @str1: The first string
 * @str2: The second string
 *
 * Return: 0 if equal, negative if str1 < str2, positive if str1 > str2
 */
int _strcmp(const char *str1, const char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2)
			return (*str1 - *str2);
		str1++;
		str2++;
	}
	return (*str1 - *str2);
}
