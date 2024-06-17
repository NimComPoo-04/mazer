#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <raylib.h>

#include "mazer.h"

void list_push(list_t *l, int move)
{
	if(l->size <= l->length)
	{
		l->size = 2 * l->size + 1;
		l->moves = realloc(l->moves, sizeof(int) * l->size);
	}

	l->moves[l->length++] = move;
}

char list_pop(list_t *l)
{
	if(l->length <= 0)
		return NOMOVE;
	else
		return l->moves[--l->length];
}

void generate_maze(mazer_t *m)
{
	extern list_t  stack;

	if(stack.length <= 0)
		return;

	int y = list_pop(&stack);
	int x = list_pop(&stack);

	if(x < 0 || y < 0 || x > m->width || y > m->height)
		return;

	if(m->cells[y * m->width + x] & VISITED)
		return;

	m->cells[y * m->width + x] |= VISITED; // mark as visited

	int current[4] = {0, 1, 2, 3};

	for(int i = 3; i > 1; i--)
	{
		int c = rand() % 4;
		int t = current[c];
		current[c] = current[i];
		current[i] = t;
	}

	for(int i = 0; i < 4; i++)
	{
		switch(current[i])
		{
			case 0:
				if(x + 1 < m->width)
				{
					m->cells[y * m->width + x]       |= RIGHT;
					m->cells[y * m->width + (x + 1)] |= LEFT;
					list_push(&stack, x + 1);
					list_push(&stack, y);
				}
				break;

			case 1:
				if(x - 1 >= 0)
				{
					m->cells[y * m->width + x]       |= LEFT;
					m->cells[y * m->width + (x - 1)] |= RIGHT;
					list_push(&stack, x - 1);
					list_push(&stack, y);
				}
				break;

			case 2:
				if(y + 1 < m->height)
				{
					m->cells[y * m->width + x]       |= DOWN;
					m->cells[(y + 1) * m->width + x] |= UP;
					list_push(&stack, x);
					list_push(&stack, y + 1);
				}
				break;

			case 3:
				if(y - 1 >= 0)
				{
					m->cells[y * m->width + x]       |= UP;
					m->cells[(y - 1) * m->width + x] |= DOWN;
					list_push(&stack, x);
					list_push(&stack, y - 1);
				}
				break;
		}
	}
}
