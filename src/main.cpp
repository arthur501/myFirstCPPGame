#include <iostream>
#include <raylib.h>

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Window Name");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

		DrawText("Congrats! You created your first window!", 190, 200, 20, RED);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}