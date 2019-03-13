#include "InputManager.h"
#include "glm\glm.hpp"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float zFar = 100.0f;
	const float zNear = 1.0f;
	const float CAM_ANGLE = 60.0f;
	const float relation = (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT;

	Game *scn = new Game(glm::vec3(0.0f, 0.0f, -1.0f), CAM_ANGLE, relation, zNear,zFar);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	init(display);
	
	scn->Init();
	
	scn->addShader("../res/shaders/basicShader");
	scn->addShader("../res/shaders/pickingShader");	
	
	display.setScene(scn);

	while(!display.closeWindow())
	{
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		scn->Draw(0,0,true);
		
		display.SwapBuffers();
		display.PollEvents();
	}
	delete scn;
	return 0;
}
