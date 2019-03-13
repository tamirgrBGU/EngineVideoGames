#define GLEW_STATIC
#include <GL\glew.h>
#include <iostream>
#include "display.h"

Display::Display(int width, int height, const std::string& title)
{
	/* Initialize the library */
    if (!glfwInit())
        error =  -1;

	m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if(!m_window)
	{
		glfwTerminate();
		error = -1;
	}
	glfwMakeContextCurrent(m_window);

	GLenum res = glewInit();
    if(res != GLEW_OK)
   {
		std::cerr << "Glew failed to initialize!" << std::endl;
    }
	isFullScreen = false;
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_SMOOTH);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT_AND_BACK);
}

void Display::addKeyCallBack(void(*keyCallback)(GLFWwindow *,int,int,int,int))
{
	glfwSetKeyCallback(m_window,(void(*)(GLFWwindow *,int,int,int,int))keyCallback);//{
	
	//glfwSetKeyCallback(m_window,[](GLFWwindow *window,int key,int scancode,int action,int mods){
	//	func(glfwGetWindowUserPointer(window),key,scancode,action,mods);
	//});
		//key_callback);
	
	

	//glEnable(GL_CULL_FACE);

}

void Display::addMouseCallBacks(void (* mousebuttonfun)(GLFWwindow*,int,int,int),void (* scrollfun)(GLFWwindow*,double,double),void (* cursorposfun)(GLFWwindow*,double,double))
{
	glfwSetMouseButtonCallback(m_window,mousebuttonfun);
	glfwSetScrollCallback(m_window, scrollfun);
	glfwSetCursorPosCallback(m_window , cursorposfun);
}

void Display::addResizeCallBack(void (*windowsizefun)(GLFWwindow*,int,int))
{
	glfwSetWindowSizeCallback(m_window,windowsizefun);
}

void Display::setScene(void *userPointer)
{
	
	glfwSetWindowUserPointer(m_window,userPointer);
}

void* Display::getScene()
{
	return glfwGetWindowUserPointer(m_window);
}

int Display::closeWindow()
{
	return glfwWindowShouldClose(m_window);
}
Display::~Display()
{
	//SDL_GL_DeleteContext(m_glContext);
	glfwDestroyWindow(m_window);
	glfwTerminate();
	//SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::IsFullscreen( void )
{
	if((glfwGetWindowMonitor( m_window ) == nullptr) != isFullScreen)
	{
		isFullScreen = (glfwGetWindowMonitor( m_window ) == nullptr);
		return true;
	}
	else 
		return false;
} 

void Display::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

void Display::PollEvents()
{
	glfwPollEvents();
}
