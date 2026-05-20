#include <iostream>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>

#include "gameMain.h"

int main(void)
{

#if PRODUCTION_BUILD == 1
	SetTraceLogLevel(LOG_NONE); // No log output to the console by raylib
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Window Name");
	SetExitKey(KEY_NULL); // Disable ESC from closing window
	SetTargetFPS(240);

	rlImGuiSetup(true);

#pragma region imgui
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enanle Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.FontGlobalScale = 1.5; // Make the ImGui Bigger

	//ImGui::StyleColorsClassic();
#pragma endregion


	if (!initGame())
	{
		return 0;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

#pragma region imgui
		rlImGuiBegin();

		// Docking stuff...
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion

		if (!updateGame())
		{
			CloseWindow();
		}

		rlImGuiEnd();

		EndDrawing();
	}

	CloseWindow();

	closeGame();

	rlImGuiShutdown();

	return 0;
}