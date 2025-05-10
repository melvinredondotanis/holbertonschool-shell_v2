#include "hsh.h"

/**
 * _isdigit - Check if a character is a digit in the specified base
 * @c: The character to check
 *
 * Return: The digit value if valid, otherwise -1
 */
static int _isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	else
		return (-1);
}

/**
 * _strtol - Convert a string to a long integer
 * @str: The string to convert
 * @endptr: Pointer to the end of the converted string
 * @base: The base to use for conversion
 *
 * Return: The converted long integer
 */
long _strtol(const char *str, char **endptr, int base)
{
	long result = 0;
	int sign = 1;

	while (_isspace((unsigned char)*str))
		str++;

	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}

	if (base == 0)
	{
		if (*str == '0')
		{
			if (*(str + 1) == 'x' || *(str + 1) == 'X')
			{
				base = 16;
				str += 2;
			}
			else
				base = 8;
		}
		else
			base = 10;
	}

	while (*str)
	{
		int digit = _isdigit(*str);

		if (digit < 0 || digit >= base)
			break;

		result = result * base + digit;
		str++;
	}

	if (endptr)
		*endptr = (char *)str;

	return (result * sign);
}
