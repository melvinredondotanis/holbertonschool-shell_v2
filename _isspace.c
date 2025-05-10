#include "hsh.h"

/**
 * _isspace - Check if a character is whitespace
 * @c: The character to check
 *
 * Return: Non-zero if whitespace, zero otherwise
 */
int _isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' ||
			c == '\f' || c == '\r');
}
