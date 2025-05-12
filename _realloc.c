#include "hsh.h"

/**
 * _realloc - Reallocates a memory block using malloc and free
 * @ptr: Pointer to the memory previously allocated
 * @size: New size of memory block in bytes
 *
 * Return: Pointer to the newly allocated memory, or NULL on failure
 */
void *_realloc(void *ptr, size_t size)
{
	void *new_ptr;
	char *src, *dest;
	size_t i;

	/* If ptr is NULL, equivalent to malloc */
	if (ptr == NULL)
		return (malloc(size));

	/* If size is 0 and ptr is not NULL, equivalent to free */
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}

	/* Allocate new memory block */
	new_ptr = malloc(size);
	if (new_ptr == NULL)
		return (NULL);

	/* Copy data from old block to new block */
	src = (char *)ptr;
	dest = (char *)new_ptr;

	/* Copy byte by byte to avoid buffer overrun */
	for (i = 0; i < size && src[i] != '\0'; i++)
		dest[i] = src[i];

	/* Free old block */
	free(ptr);

	return (new_ptr);
}
