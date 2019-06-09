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
	scn->addShader("../res/shaders/weightsShader2");

	scn->AddTexture("../res/textures/box0.bmp",false);
	scn->AddTexture("../res/textures/plane.png",true);
	scn->AddTexture(DISPLAY_WIDTH,DISPLAY_HEIGHT,COLOR);
	scn->AddTexture(DISPLAY_WIDTH,DISPLAY_HEIGHT,DEPTH);
	scn->AddBuffer(2,COLOR);
	

	scn->AddCamera(glm::vec3(0.0f, 0.0f, 1.0f),600,400, CAM_ANGLE, zNear,zFar);
	display.setScene(scn);

	while(!display.closeWindow())
	{
		scn->Draw(1,0,BACK,true,false);
		scn->Draw(1,0,COLOR,true,false);
		
		scn->Draw2D(2,1,BACK,false,false);
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();
		
	}
	delete scn;
	return 0;
}
