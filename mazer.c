#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <raylib.h>

#include "mazer.h"

void list_push(list_t *l, int move)
{
	if(l->size <= l->length)
	{
		l->size = 2 * l->size + sizeof(int);
		l->moves = realloc(l->moves, sizeof(int) * l->size);
		//printf("%d\n", l->size);
	}

	l->moves[l->length++] = move;
}

char list_pop(list_t *l)
{
	int j = 0;

	if(l->length <= 0)
		j = NOMOVE;
	else
		j = l->moves[--l->length];

	return j;
}

void generate_maze_step(mazer_t *m, list_t *stack)
{
	if(stack->length <= 0)
		return;

	int y = list_pop(stack);
	int x = list_pop(stack);

	if(x < 0 || y < 0 || x > m->width || y > m->height)
		return;

	m->cells[y * m->width + x] |= VISITED; // mark as visited

	struct {
		int x, y;
		int move1, move2;
	} neighbour[4] = {0};
	int len = 0;

	for(int i = -1; i <= 1; i++)
	{
		if(i + y < 0 || i + y >= m->height)
			continue;

		for(int j = -1; j <= 1; j++)
		{
			if(abs(i) == abs(j))
				continue;

			if(j + x < 0 || j + x >= m->width)
				continue;

			if(!(m->cells[(y + i) * m->width + (x + j)] & VISITED))
			{
				neighbour[len].x = x + j;
				neighbour[len].y = y + i;

				neighbour[len].move1 = (j == -1) * LEFT + (j == 1) * RIGHT + (i == -1) * UP + (i == 1) * DOWN; 
				neighbour[len].move2 = (j == -1) * RIGHT + (j == 1) * LEFT + (i == -1) * DOWN + (i == 1) * UP; 

				len++;
			}
		}
	}

	if(len == 0)
		return;

	list_push(stack, x);
	list_push(stack, y);

	int r = rand() % len;

	m->cells[y * m->width + x] |= neighbour[r].move1;
	m->cells[neighbour[r].y * m->width + neighbour[r].x] |= neighbour[r].move2;

	list_push(stack, neighbour[r].x);
	list_push(stack, neighbour[r].y);
}

int solve_maze(mazer_t *m, list_t *l, int x, int y)
{
	if(x == m->width - 1 && y == m->height - 1)
	{
		list_push(l, x);
		list_push(l, y);
		return 1;
	}

	if(x < 0 || y < 0 || x >= m->width || y >= m->height)
		return 0;

	if(m->cells[y * m->width + x] & SEARCHED)
		return 0;

	m->cells[y * m->width + x] |= SEARCHED;

	int b = 0;

	if(!b && m->cells[y * m->width + x] & LEFT) b = solve_maze(m, l, x - 1, y);
	if(!b && m->cells[y * m->width + x] & RIGHT) b = solve_maze(m, l, x + 1, y);
	if(!b && m->cells[y * m->width + x] & UP) b = solve_maze(m, l, x, y - 1);
	if(!b && m->cells[y * m->width + x] & DOWN) b = solve_maze(m, l, x, y + 1);

	if(b)
	{
		list_push(l, x);
		list_push(l, y);
	}

	return b;
}
