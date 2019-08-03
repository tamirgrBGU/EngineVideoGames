#include "Menu.h"



Menu::Menu(Display *display, Scene *scn, int type)
{
	this->display = display;
	this->scn = scn;
	this->created = false;
	this->move = false;
	this-> f = 0.0f;
	this->type = type;


	show_demo_window = true;
	show_another_window = false;
	clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
}

void Menu::create()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.FontGlobalScale = 1.0f;

	ImGui_ImplGlfwGL3_Init(display->getWindow(), true);
	ImGui::StyleColorsDark();
	this->created = true;

}

void Menu::DrawMenu()
{
	ImGui_ImplGlfwGL3_NewFrame();

	switch (this->type) {
	case 0:
		ImGui::Begin("logoWindow");                          // Create a window called "Hello, world!" and append into it.
		ImGui::SetWindowSize("logoWindow", ImVec2((float)200, (float)300));
		//ImGui::Image()
		//ImGui::Text("picked shape is:");//, scn->pickedShape);
		//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("velocity", &this->f, -5.0f, 5.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)               
			ImGui::Text("picked shape is:");
		//scn->tmp_test_mode = !scn->tmp_test_mode;

		ImGui::SameLine();
		//ImGui::Text("target is moving: %d", scn->tmp_test_mode);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		break;
	case 1:
		ImGui::Begin("MainWindow");                          // Create a window called "Hello, world!" and append into it.
		ImGui::SetWindowSize("MainWindow", ImVec2((float)200, (float)300));
		ImGui::Text("picked shape is:");//, scn->pickedShape);
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
																//ImGui::Checkbox("Another Window", &show_another_window);

																//ImGui::SliderFloat("velocity", &this->f, -5.0f, 5.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)               
			ImGui::Text("picked shape is:");
		//scn->tmp_test_mode = !scn->tmp_test_mode;

		ImGui::SameLine();
		//ImGui::Text("target is moving: %d", scn->tmp_test_mode);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		break;
	case 2:
		ImGui::Begin("MainWindow");                          // Create a window called "Hello, world!" and append into it.
		ImGui::SetWindowSize("MainWindow", ImVec2((float)200, (float)300));
		ImGui::Text("picked shape is:");//, scn->pickedShape);
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
																//ImGui::Checkbox("Another Window", &show_another_window);

																//ImGui::SliderFloat("velocity", &this->f, -5.0f, 5.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)               
			ImGui::Text("picked shape is:");
		//scn->tmp_test_mode = !scn->tmp_test_mode;

		ImGui::SameLine();
		//ImGui::Text("target is moving: %d", scn->tmp_test_mode);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		break;
	default:
		break;
	}
}

void Menu::destroy()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	this->created = false;

}


