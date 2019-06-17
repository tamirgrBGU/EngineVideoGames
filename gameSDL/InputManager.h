#pragma once   //maybe should be static class
#include "Display.h"
#include "game.h"


	void mouse_callback(Window* window,int button, int action, int mods)
	{	
		if(action == SDL_PRESSED )
		{
			Game *scn = (Game*)(window->userPointer);
			double x2=0,y2=0;
			SDL_Cursor *cursor = SDL_GetCursor();
		
		//	glfwGetCursorPos(window,&x2,&y2);
			scn->picking((int)x2,(int)y2);
		}
	}
	
	void scroll_callback(Window* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)(window->userPointer);
		scn->shapeTransformation(scn->zCameraTranslate,yoffset);
		
	}
	
	void key_callback(Window* window, int key, int scancode, int action, int mods)
	{
		Game *scn = (Game*)(window->userPointer);

		if(action == SDL_KEYDOWN)
		{
			switch (key)
			{
				case SDLK_SPACE:
					scn->HideShape(0);
					if(scn->IsActive())
						scn->Deactivate();
					else
						scn->Activate();
				break;
				case SDLK_RIGHT:
					//scn->shapeTransformation(scn->zGlobalRotate,-20.1f);
					scn->shapeTransformation(scn->zLocalRotate,5.1f);
//						cout<< "right: "<<endl;
					break;
				case SDLK_LEFT:
					//scn->shapeTransformation(scn->zGlobalRotate,20.1f);
					scn->shapeTransformation(scn->zLocalRotate,-5.0f);
//					cout<< "left: "<<endl;
					break;
				case SDLK_UP:
					//cout<< "up: "<<endl;
					scn->shapeTransformation(scn->xLocalRotate,5.f);
					break;
				case SDLK_DOWN:
					scn->shapeTransformation(scn->xGlobalRotate,-5.f);
					//cout<< "down: "<<endl;
					break;

			default:
				break;
			}
		}
	}
	

	void cursor_position_callback(Window* window, double xpos, double ypos)
	{
		Game *scn = (Game*)(window->userPointer);

		//scn->updatePosition((float)xpos,(float)ypos);
		//if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		//{
		//	scn->mouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		//}
		//else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		//{
		//	scn->mouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		//}

	}

	void window_size_callback(Window* window, int width, int height)
	{
		Game *scn = (Game*)(window->userPointer);

		scn->resize(width,height);
		
	}

	void init(Display &display)
	{
		display.addKeyCallBack(key_callback);
		display.addMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.addResizeCallBack(window_size_callback);

	}
