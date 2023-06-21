#include "monty.h"

void _push(stack_t **stack, unsigned int line_number);
void _pall(stack_t **stack, unsigned int line_number);
void _pint(stack_t **stack, unsigned int line_number);
void _pop(stack_t **stack, unsigned int line_number);
void _swap(stack_t **stack, unsigned int line_number);

/**
 * _push - Pushes a value to a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number of a Monty bytecodes file.
 */
void _push(stack_t **stack, unsigned int line_number)
{
	stack_t *_temp, *new;
	int i;

	new = malloc(sizeof(stack_t));
	if (new == NULL)
	{
		lastelem_op_tok_error(malloc_error());
		return;
	}

	if (op_toks[1] == NULL)
	{
		lastelem_op_tok_error(no_int_error(line_number));
		return;
	}

	for (i = 0; op_toks[1][i]; i++)
	{
		if (op_toks[1][i] == '-' && i == 0)
			continue;
		if (op_toks[1][i] < '0' || op_toks[1][i] > '9')
		{
			lastelem_op_tok_error(no_int_error(line_number));
			return;
		}
	}
	new->n = atoi(op_toks[1]);

	if (check_mode(*stack) == STACK) /* STACK mode insert at front */
	{
		_temp = (*stack)->next;
		new->prev = *stack;
		new->next = _temp;
		if (_temp)
			_temp->prev = new;
		(*stack)->next = new;
	}
	else /* QUEUE mode insert at end */
	{
		_temp = *stack;
		while (_temp->next)
			_temp = _temp->next;
		new->prev = _temp;
		new->next = NULL;
		_temp->next = new;
	}
}

/**
 * _pall - Prints the values of a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number of a Monty bytecodes file.
 */
void _pall(stack_t **stack, unsigned int line_number)
{
	stack_t *_tmp = (*stack)->next;

	while (_tmp)
	{
		printf("%d\n", _tmp->n);
		_tmp = _tmp->next;
	}
	(void)line_number;
}

/**
 * _pint - Prints the top value of a stack_t linked list.
 * @stack: A pointer to the top most node of a stack_t linked list.
 * @line_number: The current working line number
 * of a Monty bytecodes file.
 */
void _pint(stack_t **stack, unsigned int line_number)
{
	if ((*stack)->next == NULL)
	{
		lastelem_op_tok_error(pint_error(line_number));
		return;
	}

	printf("%d\n", (*stack)->next->n);
}


/**
 * _pop - Removes the top value element
 * of a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number 
 * of a Monty bytecodes file.
 */
void _pop(stack_t **stack, unsigned int line_number)
{
	stack_t *next = NULL;

	if ((*stack)->next == NULL)
	{
		lastelem_op_tok_error(pop_error(line_number));
		return;
	}

	next = (*stack)->next->next;
	free((*stack)->next);
	if (next)
		next->prev = *stack;
	(*stack)->next = next;
}

/**
 * _swap - Swaps the top two value elements of a stack_t linked list.
 * @stack: A pointer to the top mode node of a stack_t linked list.
 * @line_number: The current working line number of a Monty bytecodes file.
 */
void _swap(stack_t **stack, unsigned int line_number)
{
	stack_t *_tmp;

	if ((*stack)->next == NULL || (*stack)->next->next == NULL)
	{
		lastelem_op_tok_error(short_stack_error(line_number, "swap"));
		return;
	}

	_tmp = (*stack)->next->next;
	(*stack)->next->next = _tmp->next;
	(*stack)->next->prev = _tmp;
	if (_tmp->next)
		_tmp->next->prev = (*stack)->next;
	_tmp->next = (*stack)->next;
	_tmp->prev = *stack;
	(*stack)->next = _tmp;
}
