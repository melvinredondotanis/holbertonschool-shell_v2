#include "hsh.h"

/**
 * check_alloc - Verify memory allocation succeeded
 * @ptr: Pointer to check if allocation was successful
 *
 * This function checks if the provided pointer is NULL, indicating a failed
 * memory allocation. If allocation failed, it prints an error message and
 * exits the program with a failure status.
 *
 * Return: None (exits program on failure)
 */
void check_alloc(void *ptr)
{
	if (!ptr)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
	}
}

/**
 * check_retval - Verify a function return value
 * @retval: Return value to check (typically from system calls)
 * @msg: Error message to display if check fails
 *
 * This function checks if the provided return value is -1, which typically
 * indicates a system call failure. If the check fails, it prints the error
 * message using perror() and exits the program with a failure status.
 *
 * Return: None (exits program on failure)
 */
void check_retval(int retval, const char *msg)
{
	if (retval == -1)
	{
		perror(msg);
		exit(EXIT_FAILURE);
	}
}

/**
 * check_null - Verify a pointer is not NULL
 * @ptr: Pointer to check
 * @msg: Error message to display if pointer is NULL
 *
 * This function checks if the provided pointer is NULL. If it is,
 * it prints the provided error message to stderr and exits the
 * program with a failure status.
 *
 * Return: None (exits program on failure)
 */
void check_null(void *ptr, const char *msg)
{
	if (!ptr)
	{
		fprintf(stderr, "%s\n", msg);
		exit(EXIT_FAILURE);
	}
}
