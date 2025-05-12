#include "hsh.h"

/**
 * _memcpy - Copy memory area
 * @dest: Destination memory area
 * @src: Source memory area
 * @n: Number of bytes to copy
 *
 * Return: Pointer to the destination memory area
 */
static void *_memcpy(void *dest, const void *src, size_t n)
{
	char *d = dest;
	const char *s = src;

	while (n--)
		*d++ = *s++;
	return (dest);
}

/**
 * _realloc - Reallocate memory for a pointer
 * @ptr: Pointer to the original memory block
 * @new_size: New size for the memory block
 *
 * Return: Pointer to the reallocated memory block, or NULL on failure
 */
void *_realloc(void *ptr, size_t new_size)
{
	void *new_ptr;

	if (ptr == NULL)
		return (malloc(new_size));

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}

	new_ptr = malloc(new_size);
	if (new_ptr == NULL)
		return (NULL);

	_memcpy(new_ptr, ptr, new_size);
	free(ptr);

	return (new_ptr);
}

