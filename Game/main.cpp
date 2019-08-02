#include "InputManager.h"
#include "glm\glm.hpp"
#pragma comment(lib, "legacy_stdio_definitions.lib")
int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float zFar = 10000.0f;
	const float zNear = 1.0f;
	const float CAM_ANGLE = 60.0f;
	const float relation = (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT;

	Game *scn = new Game(glm::vec3(0.0f, 0.0f, 1.0f), CAM_ANGLE, relation, zNear,zFar);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	init(display);
	
	scn->Init();
	
	scn->addShader("../res/shaders/pickingShader");//0
	scn->addShader("../res/shaders/linearShader");//1
	scn->addShader("../res/shaders/dualQShader");//2
	scn->addShader("../res/shaders/basicShader");//3
	scn->addShader("../res/shaders/basicShadertex");//4
	scn->addShader("../res/shaders/weightsShader");//5
	scn->AddTexture("../res/textures/grass.bmp");//0
	scn->AddTexture("../res/textures/grass2.bmp");//1
	scn->AddTexture("../res/textures/grass3.bmp");//2
	//scn->AddTexture("../res/textures/snake.jpg");
	scn->AddTexture("../res/textures/snake1.png");//3
	scn->AddTexture("../res/textures/snakeEye.bmp");//4
	scn->AddTexture("../res/textures/snake2.jpg");//5
	scn->AddTexture("../res/textures/snake3.jpg");//6
	scn->AddTexture("../res/textures/snake4.jpg");//7
	scn->AddTexture("../res/textures/snake5.jpg");//8
	/*
	scn->AddTexture("../res/objs/Apple.mtl");//4
	scn->AddTexture("../res/objs/cave.mtl");//5
	scn->AddTexture("../res/objs/Nokia_3310.mtl");//5
	scn->AddTexture("../res/objs/snake_head.mtl");//6
	scn->AddTexture("../res/objs/TNT_box.mtl");//7
	*/

	display.setScene(scn);

	while(!display.closeWindow())
	{
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		scn->Draw(1,0,true);
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();
	}
	delete scn;
	return 0;
}
