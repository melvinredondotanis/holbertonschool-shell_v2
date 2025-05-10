#include "hsh.h"

/**
 * _strcat - Concatenate two strings
 * @dest: Destination string
 * @src: Source string
 *
 * Return: Pointer to the destination string
 */
char *_strcat(char *dest, const char *src)
{
	char *ptr = dest;

	while (*ptr)
		ptr++;

	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';

	return (dest);
}