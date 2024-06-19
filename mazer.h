#ifndef _MAZER_H_
#define _MAZER_H_

#define NOMOVE 0
#define UP    1
#define DOWN  2
#define LEFT  4
#define RIGHT 8

#define VISITED 16
#define SEARCHED 32

typedef struct
{
	char *cells;
	int width;
	int height;
} mazer_t;

typedef struct
{
	int *moves;
	int length;
	int size;
} list_t;

void list_push(list_t *l, int move);
char list_pop(list_t *l);

void generate_maze_step(mazer_t *m, list_t *l);
int solve_maze(mazer_t *m, list_t *l, int x, int y);

#endif
