#include "hsh.h"

/**
 * _strspn - Get the length of the initial segment of str1
 * that consists of characters in str2
 * @str1: The string to search
 * @str2: The characters to match
 *
 * Return: The length of the initial segment
 *         that consists of characters in str2
 */
static int _strspn(const char *str1, const char *str2)
{
	int count = 0;

	while (*str1 && strchr(str2, *str1))
	{
		count++;
		str1++;
	}
	return (count);
}

/**
 * _strtok_r - Tokenize a string
 * @str: The string to tokenize
 * @delim: The delimiters to use for tokenization
 * @saveptr: Pointer to the next token
 *
 * Return: The next token or NULL if no more tokens
 */
char *_strtok_r(char *str, const char *delim, char **saveptr)
{
	char *token;
	char *end;

	if (str == NULL)
		str = *saveptr;

	if (*str == '\0')
		return (NULL);

	token = str + strspn(str, delim);
	if (*token == '\0')
		return (NULL);

	end = token + strcspn(token, delim);
	if (*end != '\0')
		*end++ = '\0';

	*saveptr = end;
	return (token);
}