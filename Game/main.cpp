#include "InputManager.h"
#include "glm\glm.hpp"
#pragma comment(lib, "legacy_stdio_definitions.lib")
int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float zFar = 100.0f;
	const float zNear = 1.0f;
	const float CAM_ANGLE = 60.0f;
	const float relation = (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT;

	Game *scn = new Game(glm::vec3(0.0f, 0.0f, 1.0f), CAM_ANGLE, relation, zNear,zFar);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	init(display);
	
	scn->Init();

	scn->addShader("../res/shaders/pickingShader");
	scn->addShader("../res/shaders/linearShader");
	scn->addShader("../res/shaders/dualQShader");
	scn->addShader("../res/shaders/basicShader");
	//scn->addShader("../res/shaders/weightsShader");
	scn->addShader("../res/shaders/basicShadertex");
	scn->AddTexture("../res/textures/snake1.png");

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
