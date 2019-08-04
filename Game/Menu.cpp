#include "Menu.h"



Menu::Menu(Display *display, Game *scn, int type)
{
	this->display = display;
	this->scn = scn;
	this->created = false;
	this->move = false;
	this->type = type;
	this->show_window = true;
	this->extra_options = false;
	this->text_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
	this->sound = true;
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

		if (type != 1) {
			ImVec2 windowSize = ImVec2(ImGui::GetIO().DisplaySize.x - 50, ImGui::GetIO().DisplaySize.y - 60); // My ImGui window as fullscreen - padding (x=50, y=60)
			ImVec2 buttonSize(60, 30); // My button size
		}
			ImGuiStyle& style = ImGui::GetStyle();
		

		switch (this->type) {
		case 0:
			
			ImGui::Begin("logoWindow", &show_window, ImVec2((float)1200, (float)800), -1.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - 615)*0.5f, (ImGui::GetWindowSize().y - 352)* 0.2f));
			ImGui::Image((GLuint*) 5, ImVec2(615, 352));
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - 800)*0.5f, (ImGui::GetWindowSize().y - 89)* 0.6f));
			ImGui::Image((GLuint*) 12, ImVec2(800, 89));
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - 200)*0.5f, (ImGui::GetWindowSize().y - 63)* 0.8f));
			if (ImGui::ImageButton((GLuint*)6, ImVec2(200, 63))) {
				scn->playWelcome();
				this->show_window = false;
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			break;
		case 1:
			ImGui::Begin("score", &show_window, ImVec2((float)200, (float)800), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove );
			ImGui::SetWindowSize("score", ImVec2((float)350, (float)500));
			style.WindowBorderSize = 0.0f;
			
			if (ImGui::Button(this->extra_options?"Resume":"Options")) {
				printf("clicked on options\n");
				this->extra_options ? this->scn->Activate() : this->scn->Deactivate();
				this->extra_options = !this->extra_options;
			}
			if (this->extra_options || !this->scn->IsActive()) {
				this->sound = this->scn->getSoundVar();
				if (ImGui::Checkbox("Mute sounds", &this->sound)) {
					this->scn->switchSoundEnable();
				};
					
				if (ImGui::Button("Quit")) {
					printf("clicked on quit\n");
					this->display->quit_game = true;
				}
				
				ImGui::Begin("pause", &show_window, ImVec2((float)1200, (float)800), 0.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);	
				ImGui::SetCursorPos(ImVec2(500,300));
				ImGui::SetWindowFontScale(5.0f);
				ImGui::TextColored(ImVec4(0.28f, 0.79f, 0.52f, 1.0f), "Paused");
				ImGui::End();
			}
			ImGui::SetWindowFontScale(3.5f);
			ImGui::Image((GLuint*)(this->scn->currentTheme+7), ImVec2(50, 50));
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.28f, 0.79f, 0.52f,1.0f),"%d / %d", this->scn->getTotalFruitCount(), this->scn->getCurrentFruitCount());

			
			ImGui::End();
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
			break;
		case 2:
			ImGui::Begin("winWindow", &show_window, ImVec2((float)1200, (float)800), -1.0f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - 615)*0.5f, (ImGui::GetWindowSize().y - 252)* 0.2f));
			if (scn->wonGame)
				ImGui::Image((GLuint*)20, ImVec2(615, 252));
			else
				ImGui::Image((GLuint*)21, ImVec2(615, 252));
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - 800)*0.5f, (ImGui::GetWindowSize().y - 89)* 0.6f));
			ImGui::Image((GLuint*)12, ImVec2(800, 89));
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


