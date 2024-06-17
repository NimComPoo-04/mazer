#include <stdlib.h>
#include <raylib.h>
#include <time.h>

#include "mazer.h"

#define WIDTH 100
#define HEIGHT 100

int main(void)
{
	mazer_t m = {0};

	m.width = WIDTH; 
	m.height = HEIGHT; 

	m.cells = calloc(sizeof(char), m.width * m.height);

	list_t l = {0};

	list_push(&l, 0);
	generate_maze(&m, &l);
//	list_t moves = solve_maze(&m);

	InitWindow(800, 800, "Maze");

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
					if(!(m.cells[v] & UP))    DrawLineEx((Vector2){j * cw, i * ch}, (Vector2){(j + 1) * cw, i * ch}, 3, RED);
					if(!(m.cells[v] & DOWN))  DrawLineEx((Vector2){j * cw, (i + 1) * ch}, (Vector2){(j + 1) * cw, (i + 1) * ch}, 3, RED);
					if(!(m.cells[v] & LEFT))  DrawLineEx((Vector2){j * cw, i * ch}, (Vector2){j * cw, (i + 1) * ch}, 3, RED);
					if(!(m.cells[v] & RIGHT)) DrawLineEx((Vector2){(j + 1) * cw, i * ch}, (Vector2){(j + 1) * cw, (i + 1) * ch}, 3, RED);
				}
			}
		}

		static int space_down = 0;
		if(IsKeyDown(KEY_SPACE) && !space_down)
		{
			generate_maze(&m, &l);
			space_down = 1;
		}
		if(IsKeyUp(KEY_SPACE))
			space_down = 0;

		static int q_down = 0;
		if(IsKeyDown(KEY_Q) && !q_down)
		{
			TraceLog(LOG_INFO, "--------------");
			for(int i = 0; i < l.length; i++)
			{
				int x = l.moves[i] % m.width;
				int y = l.moves[i] / m.width;
				TraceLog(LOG_INFO, "x: %d   y: %d", x, y);
			}
			q_down = 1;
		}
		if(IsKeyUp(KEY_Q))
			q_down = 0;

		/*
		for(int i = 0; i < moves.length; i++)
		{
			int v = moves.moves[i];

			int x = v % m.width;
			int y = v / m.width;

			DrawRectangle(x * cw, y * ch, cw, ch, DARKBROWN);
		}
		*/

		DrawFPS(3, 3);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
