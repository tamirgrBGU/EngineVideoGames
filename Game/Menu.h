#pragma once
#include "display.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_glfw_gl3.h"
#include "scene.h"
#include "game.h"

class Menu
{
public:
	Display *display;
	Game *scn;
	bool show_window;
	bool extra_options;
	bool move ;
	bool created;
	float f;
	int type;
	ImVec4 text_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	Menu(Display *display, Game *scn, int type);
	void create();
	void DrawMenu();
	void destroy();
	bool sound;
	
};

