#include <iostream>
#include <raylib.h>

#include <imgui.h>
#include <rlImGui.h>

int main()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "Window Name");

	rlImGuiSetup(true);

#pragma region imgui
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enanle Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.FontGlobalScale = 1.5; // Make the ImGui Bigger

	//ImGui::StyleColorsClassic();
#pragma endregion

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);

#pragma region imgui
		rlImGuiBegin();

		// Docking stuff...
		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion

		DrawText("Congrats! You created your first window!", 190, 200, 20, RED);

#pragma region imgui
		ImGui::Begin("Test");
		
		ImGui::Text("Hello");

		if (ImGui::Button("Button"))
		{
			std::cout << "Pressed\n";
		}
		ImGui::SameLine();

		ImGui::PushID(2);
		if (ImGui::Button("Button"))
		{
			std::cout << "Second Button\n";
		}
		ImGui::PopID();

		ImGui::End();

		rlImGuiEnd();
#pragma endregion

		EndDrawing();
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}