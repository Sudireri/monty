/*
 * File: run_montyrun.c
 * Auth: Sudi Ireri
 *       Reagan Mamesa
 */
#include <stdio.h>
#include "monty.h"
#include <stdlib.h>
#include <string.h>

void token_free(void);
unsigned int optok_len(void);
int delim_check(char *line, char *delims);
void (*get_op_func(char *opcode))(stack_t**, unsigned int);
int monty_code(FILE *script_fd);

/**
 * token_free - Frees op_toks.
 * Return:Nothing
 */
void token_free(void)
{
	size_t count = 0;

	if (op_toks == NULL)
		return;

	for (count = 0; op_toks[count]; count++)
		free(op_toks[count]);

	free(op_toks);
}

/**
 * optok_len - Gets the length of current op_toks.
 *
 * Return: Length of current op_toks (as int).
 */
unsigned int optok_len(void)
{
	unsigned int _len = 0;

	while (op_toks[_len])
		_len++;
	return (_len);
}

/**
 * delim_check - Checks if a line read from getline only contains delimiters.
 * @line: A pointer to the line.
 * @del: A string of delimiter characters.
 *
 * Return: If the line only contains delimiters - 1.
 *         Otherwise - 0.
 */
int delim_check(char *line, char *del)
{
	int i, j;

	for (i = 0; line[i]; i++)
	{
		for (j = 0; del[j]; j++)
		{
			if (line[i] == del[j])
				break;
		}
		if (del[j] == '\0')
			return (0);
	}

	return (1);
}

/**
 * get_op_func - Matches an opcode with its corresponding function.
 * @opcode: The opcode to match.
 *
 * Return: A pointer to the corresponding function.
 */
void (*get_op_func(char *opcode))(stack_t**, unsigned int)
{
	instruction_t op_funcs[] = {
		{"push", _push},
		{"pall", _pall},
		{"pint", _pint},
		{"pop", _pop},
		{"swap", _swap},
		{"add", _add},
		{"nop", _nop},
		{"sub", _sub},
		{"div", _div},
		{"mul", _mul},
		{"mod", _mod},
		{"pchar", _pchar},
		{"pstr", _pstr},
		{"rotl", monty_rotl},
		{"rotr", monty_rotr},
		{"stack", monty_stack},
		{"queue", monty_queue},
		{NULL, NULL}
	};
	int i;

	for (i = 0; op_funcs[i].opcode; i++)
	{
		if (strcmp(opcode, op_funcs[i].opcode) == 0)
			return (op_funcs[i].f);
	}

	return (NULL);
}

/**
 * monty_code - Primary function to execute a Monty bytecodes script.
 * @script_fd: File descriptor for an open Monty bytecodes script.
 *
 * Return: EXIT_SUCCESS on success, respective error code on failure.
 */
int monty_code(FILE *script_fd)
{
	stack_t *orig_stack = NULL;
	char line[120];/* = NULL;*/
	size_t len = 120, exit_status = EXIT_SUCCESS;
	unsigned int line_number = 0, prev_tok_len = 0;
	void (*op_func)(stack_t**, unsigned int);

	if (init_stack(&orig_stack) == EXIT_FAILURE)
		return (EXIT_FAILURE);

	while (fgets(line, len, script_fd) != NULL)
	{
		line_number++;
		op_toks = strtow(line, DELIMS);
		if (op_toks == NULL)
		{
			if (delim_check(line, DELIMS))
				continue;
			free_stack(&orig_stack);
			return (malloc_error());
		}
		else if (op_toks[0][0] == '#') /* comment line */
		{
			token_free();
			continue;
		}
		op_func = get_op_func(op_toks[0]);
		if (op_func == NULL)
		{
			free_stack(&orig_stack);
			exit_status = unknown_op_error(op_toks[0], line_number);
			token_free();
			break;
		}
		prev_tok_len = optok_len();
		op_func(&orig_stack, line_number);
		if (optok_len() != prev_tok_len)
		{
			if (op_toks && op_toks[prev_tok_len])
				exit_status = atoi(op_toks[prev_tok_len]);
			else
				exit_status = EXIT_FAILURE;
			token_free();
			break;
		}
		token_free();
	}
	free_stack(&orig_stack);

	if (line[0]=='\0')/* && *line == 0)*/
	{
		/*free(line);*/
		return (malloc_error());
	}

	/*free(line);*/
	return (exit_status);
}
