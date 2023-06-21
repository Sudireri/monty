#include "monty.h"

/**
 * lastelem_op_tok_error - Sets last element of op_toks to be an error code.
 * @error_code: Integer to store as a string in op_toks.
 */
void lastelem_op_tok_error(int error_code)
{
	int len_of_tok = 0, i = 0;
	char *exit_str = NULL;
	char **new_toks = NULL;

	len_of_tok = optok_len();
	new_toks = malloc(sizeof(char *) * (len_of_tok + 2));
	if (!new_toks)
	{
		malloc_error();
		return;
	}
	while (i < len_of_tok)
	{
		new_toks[i] = op_toks[i];
		i++;
	}
	if (op_toks[i])
		free(op_toks[i]);
	exit_str = char_get(error_code);
	if (!exit_str)
	{
		free(new_toks);
		malloc_error();
		return;
	}
	new_toks[i++] = exit_str;
	new_toks[i] = NULL;
	free(op_toks);
	op_toks = new_toks;
}
