#include "hsh.h"

/**
 * _strcpy - Copy a string from src to dest
 * @dest: Destination string
 * @src: Source string
 *
 * Return: Pointer to the destination string
 */
char *_strcpy(char *dest, const char *src)
{
	int i;

	if (!dest || !src)
		return (NULL);

	for (i = 0; src[i] != '\0'; i++)
		dest[i] = src[i];

	dest[i] = '\0';

	return (dest);
}
