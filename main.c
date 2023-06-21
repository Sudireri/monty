#include "monty.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char **op_toks = NULL;

/**
 * main - Monty Interpreter entry point
 *
 * @argc: the count of arguments passed to the program
 * @argv: pointer to an array of char pointers to arguments
 *
 * Return: (EXIT_SUCCESS) on success (EXIT_FAILURE) on error
 */
int main(int argc, char **argv)
{
	FILE *file_script_fd = NULL;
	int status_exit_code = EXIT_SUCCESS;

	if (argc != 2)
		return (usage_error());
	file_script_fd = fopen(argv[1], "r");
	if (file_script_fd == NULL)
		return (f_open_error(argv[1]));
	status_exit_code = monty_code(file_script_fd);
	fclose(file_script_fd);
	return (status_exit_code);
}
