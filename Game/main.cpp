#include "InputManager.h"
#include "glm\glm.hpp"
#include "Menu.h"
#pragma comment(lib, "legacy_stdio_definitions.lib")

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float zFar = 100000.0f;
	const float zNear = 1.0f;
	const float CAM_ANGLE = 60.0f;
	const float relation = (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT;
	Game *scn = new Game(glm::vec3(0.0f, 0.0f, 1.0f), CAM_ANGLE, relation, zNear,zFar);

	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	init(display);
	
	scn->addShader("../res/shaders/pickingShader");//0
	scn->addShader("../res/shaders/linearShader");//1
	scn->addShader("../res/shaders/dualQShader");//2
	scn->addShader("../res/shaders/basicShader");//3
	scn->addShader("../res/shaders/basicShadertex");//4
	scn->addShader("../res/shaders/weightsShader");//5
	scn->addShader("../res/shaders/basicShadertexNoShadow");//5
	
	scn->AddTexture("../res/textures/snakeEye.bmp");//0
	scn->AddTexture("../res/textures/Tongue.png");//1
	scn->AddTexture("../res/textures/grass2.bmp");//2	// NOT USED
	scn->AddTexture("../res/textures/grass3.bmp");//3	// NOT USED
	
	scn->AddTexture("../res/textures/logo.png");//4
	scn->AddTexture("../res/textures/start.png");//5
	scn->AddTexture("../res/textures/fruit1.png");//6
	scn->AddTexture("../res/textures/fruit2.png");//7
	scn->AddTexture("../res/textures/fruit3.png");//8
	scn->AddTexture("../res/textures/fruit4.png");//9
	scn->AddTexture("../res/textures/fruit5.png");//10
	scn->AddTexture("../res/textures/credits.png");//11
	scn->AddTexture("../res/textures/sky-up.jpg");//12
	scn->AddTexture("../res/textures/sky-front.jpg");//13
	scn->AddTexture("../res/textures/sky-right.jpg");//14
	scn->AddTexture("../res/textures/sky-back.jpg");//15
	scn->AddTexture("../res/textures/sky-left.jpg");//16
	scn->AddTexture("../res/textures/sky-bottom.jpg");//17

	scn->Init();

	display.setScene(scn);

	Menu* menu = new Menu(&display, scn, 0); //start menu
	Menu* menu2 = new Menu(&display, scn, 1); //fruits count and options - his name is fruitCounter in game.h private


	while(!display.closeWindow() && !display.quit_game)
	{
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		scn->Draw(1,0,true);
		scn->Motion();

		if (!menu->created)
		{
			menu->create();
			
			init(display);
			scn->PlayTheme();
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
