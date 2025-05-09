#include "hsh.h"

/**
 * _strncmp - Compare two strings up to n characters
 * @str1: The first string
 * @str2: The second string
 * @n: The number of characters to compare
 *
 * Return: 0 if equal, negative if str1 < str2, positive if str1 > str2
 */
int _strncmp(const char *str1, const char *str2, size_t n)
{
	size_t i;

	for (i = 0; i < n && str1[i] && str2[i]; i++)
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
	if (i < n)
		return (str1[i] - str2[i]);
	return (0);
}