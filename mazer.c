#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <raylib.h>

#include "mazer.h"

void list_push(list_t *l, int move)
{
	if(l->size <= l->length)
	{
		l->size = 3 * l->size / 2 + 1;
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


int generate_maze_rec(mazer_t *m, int x, int y)
{
#define CELLAT(_x_, _y_)  m->cells[((_x_) + (_y_) * m->width)]

	if(x >= m->width || x < 0 || y < 0 || y >= m->height)
		return 0;

	if(m->cells[y * m->width + x] & VISITED)
		return 0;

	m->cells[y * m->width + x] |= VISITED;

	struct mazemove
	{
		int dx; int dy;
		char move1; char move2;
	} current[4] = {
		{1, 0, RIGHT, LEFT},
		{-1, 0, LEFT, RIGHT},
		{0, 1, DOWN, UP},
		{0, -1, UP, DOWN}
	};

	for(int i = 3; i > 1; i--)
	{
		int c = rand() % i;
		struct mazemove mov = current[c];
		current[c] = current[i];
		current[i] = mov;
	}

	int i = 0;
	for(i = 0; i < 4; i++)
	{
		if(current[i].dx + x >= 0 && current[i].dx + x < m->width &&
				current[i].dy + y >= 0 && current[i].dy + y < m->height &&
				!(CELLAT(current[i].dx + x, current[i].dy + y) & VISITED))
		{
			CELLAT(x, y) |= current[i].move1;
			CELLAT(x + current[i].dx, y + current[i].dy) |= current[i].move2;

			generate_maze_rec(m, x + current[i].dx, y + current[i].dy);
		}
	}

#undef CELLAT

	return 1;
}

void generate_maze(mazer_t *m, list_t *l)
{
	srand(time(0));
	generate_maze_rec(m, 0, 0);
}

int solve_maze_rec(mazer_t *m, list_t *l, int x, int y)
{
#define CELLAT(_x_, _y_)  m->cells[((_x_) + (_y_) * m->width)]
	if(x < 0 || x >= m->width || y < 0 || y >= m->height)
		return 0;

	if(CELLAT(x, y) & SEARCHED)
		return 0;

	if(x == m->width - 1 && y == m->height - 1)
	{
		list_push(l, y * m->width + x);
		return 1;
	}

	CELLAT(x, y) |= SEARCHED;

	int b = 0;

	if(CELLAT(x, y) & LEFT && solve_maze_rec(m, l, x - 1, y)) b = 1;
	if(CELLAT(x, y) & RIGHT && solve_maze_rec(m, l, x + 1, y)) b = 1;
	if(CELLAT(x, y) & UP && solve_maze_rec(m, l, x, y - 1)) b = 1;
	if(CELLAT(x, y) & DOWN && solve_maze_rec(m, l, x, y + 1)) b = 1;


	if(b) list_push(l, y * m->width + x);
#undef CELLAT

	return b;
}

list_t solve_maze(mazer_t *m)
{
	//TODO: Use DFS to solve the maze and return the solution
	list_t l = {0};
	solve_maze_rec(m, &l, 0, 0);
	return l;
}
