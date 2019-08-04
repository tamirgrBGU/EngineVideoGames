#include "Menu.h"



Menu::Menu(Display *display, Scene *scn, int type)
{
	this->display = display;
	this->scn = scn;
	this->created = false;
	this->move = false;
	this-> f = 0.0f;
	this->type = type;
	this->sounds = sounds;
	show_window = true;
	extra_options = false;
	text_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
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
	if (this->show_window) {
		ImGui_ImplGlfwGL3_NewFrame();

		if (type == 0) {
			ImVec2 windowSize = ImVec2(ImGui::GetIO().DisplaySize.x - 50, ImGui::GetIO().DisplaySize.y - 60); // My ImGui window as fullscreen - padding (x=50, y=60)
			ImVec2 buttonSize(60, 30); // My button size
		}
			ImGuiStyle& style = ImGui::GetStyle();
		

		switch (this->type) {
		case 0:
			ImGui::Begin("logoWindow", &show_window, ImVec2((float)1200, (float)800),-1.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);                          // Create a window called "Hello, world!" and append into it.
			//ImGui::SetWindowSize("logoWindow", ImVec2((float)1200, (float)800));
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - 615)*0.5f, (ImGui::GetWindowSize().y - 352)* 0.5f));
			ImGui::Image((GLuint*)5, ImVec2(615, 352));
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - 200)*0.5f, (ImGui::GetWindowSize().y - 63)* 0.8f));
			if (ImGui::ImageButton((GLuint*)6, ImVec2(200, 63)))
				this->show_window = false;

			ImGui::End();

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			break;
		case 1:
			ImGui::Begin("score", &show_window, ImVec2((float)200, (float)800), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove );
			ImGui::SetWindowSize("score", ImVec2((float)300, (float)500));
			//ImGui::SetNextWindowBgAlpha(0.0f);
			
			style.WindowBorderSize = 0.0f;
			
			if (ImGui::Button(this->extra_options?"Resume":"Options")) {
				printf("clicked on options\n");
				this->extra_options = !this->extra_options;
			}
			if (this->extra_options) {
				ImGui::Checkbox("Mute sounds", this->sounds);
				if (ImGui::Button("Quit")) {
					printf("clicked on quit\n");
					this->display->quit_game = true;
				}
				
				ImGui::Begin("pause", &show_window, ImVec2((float)1200, (float)800), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);	
				ImGui::SetCursorPos(ImVec2(500,300));
				ImGui::SetWindowFontScale(3.5f);
				ImGui::TextColored(ImVec4(0.28f, 0.79f, 0.52f, 1.0f), "Paused");
				ImGui::End();
			}
			ImGui::SetWindowFontScale(2.5f);
			ImGui::Image((GLuint*)7, ImVec2(50, 50));
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.28f, 0.79f, 0.52f,1.0f),"%d / 3", 0);

			
			ImGui::End();
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			break;
		case 2:
			ImGui::Begin("MainWindow", &show_window, ImVec2((float)200, (float)300));                          // Create a window called "Hello, world!" and append into it.
			//ImGui::SetWindowSize("MainWindow", );
			ImGui::Text("picked shape is:");//, scn->pickedShape);
			ImGui::Checkbox("Demo Window", &show_window);      // Edit bools storing our window open/close state
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
}

void Menu::destroy()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	this->created = false;

}


