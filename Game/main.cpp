#include "InputManager.h"
#include "glm\glm.hpp"
#include "Menu.h"
#pragma comment(lib, "legacy_stdio_definitions.lib")

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float zFar = 10000.0f;
	const float zNear = 1.0f;
	const float CAM_ANGLE = 60.0f;
	const float relation = (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT;
	//bool started = true;
	Game *scn = new Game(glm::vec3(0.0f, 0.0f, 1.0f), CAM_ANGLE, relation, zNear,zFar);
	bool allow_sounds = true;
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	init(display);
	
	scn->addShader("../res/shaders/pickingShader");//0
	scn->addShader("../res/shaders/linearShader");//1
	scn->addShader("../res/shaders/dualQShader");//2
	scn->addShader("../res/shaders/basicShader");//3
	scn->addShader("../res/shaders/basicShadertex");//4
	scn->addShader("../res/shaders/weightsShader");//5
	scn->AddTexture("../res/textures/grass.bmp");//0
	scn->AddTexture("../res/textures/grass2.bmp");//1
	scn->AddTexture("../res/textures/grass3.bmp");//2
	scn->AddTexture("../res/textures/snakeEye.bmp");//3
	
	scn->AddTexture("../res/textures/logo.png");//9
	scn->AddTexture("../res/textures/start.png");//10
	scn->AddTexture("../res/textures/fruit1.png");//11
	scn->AddTexture("../res/textures/fruit2.png");//11
	scn->AddTexture("../res/textures/fruit3.png");//11
	scn->AddTexture("../res/textures/fruit4.png");//11
	scn->Init();

	display.setScene(scn);

	Menu* menu = new Menu(&display, scn, 0, &allow_sounds); //start menu
	Menu* menu2 = new Menu(&display, scn, 1, &allow_sounds); //fruits count
	//Menu* menu3 = new Menu(&display, scn, 2, &allow_sounds); //option menu



	while(!display.closeWindow() && !display.quit_game)
	{
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		scn->Draw(1,0,true);
		scn->Motion();
		if (!menu->created)
		{
			menu->create();
			init(display);
		}
		if (menu->show_window)
			menu->DrawMenu();

		if (!menu->created)
		{
			menu2->create();
			//init(display);
		}
		if (!menu->show_window)
			menu2->DrawMenu();

		display.SwapBuffers();
		display.PollEvents();
	}
	delete scn;
	return 0;
}
