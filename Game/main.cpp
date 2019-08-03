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
	
	scn->Init();

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
