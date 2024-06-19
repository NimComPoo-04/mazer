#include <stdlib.h>
#include <time.h>
#include <raylib.h>

#include "mazer.h"

#define WIDTH 100
#define HEIGHT 100

void draw_path(list_t *l, int cw, int ch, Color col)
{
	for(int i = 0; i < l->length - 4; i+=2)
	{
		Vector2 start = {l->moves[i] * cw + cw/2, l->moves[i + 1] * ch + ch/2};
		Vector2 end = {l->moves[i + 2] * cw + cw/2, l->moves[i + 3] * ch + ch/2};

		DrawLineEx(start, end, 3, col);
	}
}

int main(void)
{
	srand(time(0));

	mazer_t m = {0};

	m.width = WIDTH; 
	m.height = HEIGHT; 

	m.cells = calloc(sizeof(char), 100 + m.width * m.height);

	list_t stack = {0};
	list_push(&stack, 0);	// x value
	list_push(&stack, 0);	// y value
				//
	// SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 800, "Maze");

	list_t path = {0};

	while(!WindowShouldClose())
	{
		PollInputEvents();

		BeginDrawing();	

		ClearBackground(RAYWHITE);

		int cw = GetScreenWidth() / m.width;
		int ch = GetScreenHeight() / m.height;

		for(int i = 0; i < m.height; i++)
		{
			for(int j = 0; j < m.width; j++)
			{
				int v = i * m.width + j;

				if(m.cells[v] == NOMOVE)
					DrawRectangleLines(j * cw, i * ch, cw, ch, LIGHTGRAY);
				else
				{
					if(!(m.cells[v] & UP))    DrawLineEx((Vector2){j * cw, i * ch}, (Vector2){(j + 1) * cw, i * ch}, 2.5, RED);
					if(!(m.cells[v] & DOWN))  DrawLineEx((Vector2){j * cw, (i + 1) * ch}, (Vector2){(j + 1) * cw, (i + 1) * ch}, 2.5, RED);
					if(!(m.cells[v] & LEFT))  DrawLineEx((Vector2){j * cw, i * ch}, (Vector2){j * cw, (i + 1) * ch}, 2.5, RED);
					if(!(m.cells[v] & RIGHT)) DrawLineEx((Vector2){(j + 1) * cw, i * ch}, (Vector2){(j + 1) * cw, (i + 1) * ch}, 2.5, RED);
				}
			}
		}

		static int on = 0;
		if(on)
		{
			if(stack.length != 0)
			{
				generate_maze_step(&m, &stack);
				draw_path(&stack, cw, ch, GREEN);
			}
			else if(path.length == 0)
				solve_maze(&m, &path, 0, 0);
		}
		else
		{
			DrawText("Maze Generation", 60, 280, 80, LIME);
			DrawText("100 x 100 Grid", 140, 280 + 80 + 40, 80, LIME);
		}
		

		if(IsKeyDown(KEY_SPACE))
			on = 1;

		draw_path(&path, cw, ch, PINK);

//		DrawFPS(3, 3);

		EndDrawing();
	}

	return 0;
}
