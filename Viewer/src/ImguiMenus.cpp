 #pragma once
#include "ImguiMenus.h"
#include <stdio.h>
#include <stdlib.h>
// open file dialog cross platform https://github.com/mlabbe/nativefiledialog
#include <nfd.h>
//my includes:
#include <iostream>
#include <string>
#include <algorithm>

bool showDemoWindow = false;
bool showAnotherWindow = false;
bool showFile = false;

bool modelsWindow = false;
bool camerasWindow = false;
glm::vec4 clearColor = glm::vec4(0.4f, 0.55f, 0.60f, 1.00f);

const glm::vec4& GetClearColor()
{
	return clearColor;
}

void DrawImguiMenus(ImGuiIO& io, Scene* scene)
{
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		ImGui::Begin("Test Menu");
		static float f = 0.0f;
		static int counter = 0;
		// Display some text (you can use a format string too)
		ImGui::Text("Hello, world!\nHere you can change which model will be viewed and view stats about the model");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our windows open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);
		ImGui::Checkbox("Models Window", &modelsWindow);

		if (ImGui::Button("Button")) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (showAnotherWindow)
	{
		int val[2]; val[0] = io.MousePos.x; val[1] = io.MousePos.y;
		ImGui::Begin("Another Window", &showAnotherWindow);
		ImGui::InputInt2("(x,y)", val, 3);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			showAnotherWindow = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (showDemoWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						ImGui::Text("Hello from another window!");
						std::cout << "success\n";
						
						std::string filename(outPath);
						std::cout << filename << std::endl;
						//replace all '\' to '/' in the path
						std::replace(filename.begin(), filename.end(), '\\', '/');
						std::cout << filename << std::endl;
						scene->LoadOBJModel(filename);
						std::cout << "test LoadOBJ\n";
						std::cout << "active Model: " << scene->ActiveModel << std::endl;
						std::cout << "num of models: " << scene->getModels().size() << std::endl;
						std::cout << "Models[0] " << scene->getModels()[0]->getNameModel() << std::endl;
						std::cout << "Models[1] " << scene->getModels()[1]->getNameModel() << std::endl;


						free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
						std:cout << "why...?";
					}

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Demo Menu")) { showDemoWindow = true; }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	if (modelsWindow)
	{
		ImGui::Begin("Models", &modelsWindow);


		ImGui::End();
		
	}
}