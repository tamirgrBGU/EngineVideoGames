#include "InputManager.h"
#include "glm\glm.hpp"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float zFar = 100.0f;
	const float zNear = 1.0f;
	const float CAM_ANGLE = 60.0f;

	Game *scn = new Game(glm::vec3(0.0f, 0.0f, 1.0f),DISPLAY_WIDTH,DISPLAY_HEIGHT, CAM_ANGLE, zNear,zFar);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	init(display);
	
	scn->Init();

	scn->addShader("../res/shaders/pickingShader");	
	scn->addShader("../res/shaders/basicShader");
	//scn->addShader("../res/shaders/basicShadertex");
	scn->AddTexture("../res/textures/box0.bmp");
	scn->AddTexture("../res/textures/plane.png");
	scn->AddCamera(glm::vec3(0.0f, 0.0f, 1.0f),300,300, CAM_ANGLE, zNear,zFar);
	display.setScene(scn);

	while(!display.closeWindow())
	{
		scn->Draw(1,0,true);
		scn->Draw(1,1,false);
		scn->Motion();
		display.SwapBuffers();
		
		display.PollEvents();
	}
	delete scn;
	return 0;
}
