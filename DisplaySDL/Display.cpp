#define GLEW_STATIC
#include "Display.h"
#include <iostream>
#include <GL/glew.h>

Display::Display(int width, int height, const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

	m_window.window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window.window);

	GLenum res = glewInit();
    if(res != GLEW_OK)
    {
		std::cerr << "Glew failed to initialize!" << std::endl;
    }
	m_isclosed = false;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	m_window.userPointer = 0;
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window.window);
	SDL_Quit();
}

void  Display::PollEvents()
{
	SDL_Event e;
	if(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_QUIT:
			m_isclosed = true;
			break;
		case SDL_KEYDOWN:
			if(e.key.keysym.scancode == SDLK_ESCAPE)
				m_isclosed = true;
			else
				//window,key,scancode,action,mode
				keyFunc(&m_window,e.key.keysym.sym,e.key.keysym.scancode,SDL_KEYDOWN,e.key.keysym.mod);
			break;
		case SDL_KEYUP:
			//window,key,scancode,action,mode
			keyFunc(&m_window,e.key.keysym.sym,e.key.keysym.scancode,SDL_KEYUP,e.key.keysym.mod);
		break;
		case SDL_MOUSEMOTION:
			cursorPosFunc(&m_window,e.motion.x,e.motion.y);
		break;
		case SDL_MOUSEBUTTONDOWN:
			//window, button, action,mode
			mouseFunc(&m_window,e.button.button,SDL_MOUSEBUTTONDOWN,e.button.state);
			break;
		case SDL_MOUSEBUTTONUP:
			//window, button, action,mode
			mouseFunc(&m_window,e.button.button,SDL_MOUSEBUTTONUP,e.button.state);
		break;
		case SDL_WINDOWEVENT:
			if(e.window.event == SDL_WINDOWEVENT_RESIZED)
				resizeFunc(&m_window,e.window.data1,e.window.data2);
		break;
		
		default:
			break;
		}
		
	}
}
	bool  Display::IsFullscreen(  )
	{
		return false;
	}
	void*  Display::getScene()
	{
		return m_window.userPointer;
	}

	void  Display::setScene(void *userPointer)
	{
		m_window.userPointer = userPointer;
	}	

	void  Display::addKeyCallBack(void(*func)(Window *,int,int,int,int))
	{
	    keyFunc = func;
	}
	void  Display::addMouseCallBacks(void (* mousebuttonfun)(Window*,int,int,int),void(* scrollfun)(Window*,double,double),void (* cursorposfun)(Window*,double,double))
	{
		mouseFunc = mousebuttonfun;
		cursorPosFunc = cursorposfun;
		scrollFunc = scrollfun;
	}

	void  Display::addResizeCallBack(void (*windowsizefun)(Window*,int,int))
	{
		resizeFunc = windowsizefun;
	}

	void Display::SwapBuffers()
	{
		SDL_GL_SwapWindow(m_window.window);
	}

