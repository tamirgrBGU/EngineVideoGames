#include <iostream>

#include "InputManager.h"
#include <Windows.h>

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 1200;
	const int DISPLAY_HEIGHT = 800;
	const float zFar = 100.0f;
	const float zNear = 1.0f;
	const float CAM_ANGLE = 60.0f;
	float relation = (float)DISPLAY_WIDTH/(float)DISPLAY_HEIGHT;
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	init(display);
	Game scn(glm::vec3(0.0f, 5.0f, -20.0f), CAM_ANGLE, relation, zNear,zFar);
	scn.Init();
	
	scn.addShader("../res/shaders/basicShader");
	scn.addShader("../res/shaders/pickingShader");	

	display.setScene(&scn);
	while(!display.closeWindow())
	{
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);

		scn.draw(0,0,true);

		display.SwapBuffers();
		display.PollEvents();
	}
	

	return 0;
}
