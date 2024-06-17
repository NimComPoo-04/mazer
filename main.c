#include <stdlib.h>
#include <raylib.h>

#include "mazer.h"

#define WIDTH 100
#define HEIGHT 100

list_t stack;

int main(void)
{
	mazer_t m = {0};

	m.width = WIDTH; 
	m.height = HEIGHT; 

	m.cells = calloc(sizeof(char), m.width * m.height);

	stack = (list_t){
		.moves = calloc(sizeof(int), 32),
		.length = 0,
		.size = 32
	};

	list_push(&stack, 0);	// x value
	list_push(&stack, 0);	// y value
				//
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

		for(int i = 0; i < stack.length; i+=2)
		{
			int x = stack.moves[i];
			int y = stack.moves[i + 1];

			DrawRectangle(x * cw, y * ch, cw, ch, PURPLE);
		}

		if(IsKeyDown(KEY_SPACE))
			generate_maze(&m);

		DrawFPS(3, 3);

		EndDrawing();
	}

	// CloseWindow();

	return 0;
}
