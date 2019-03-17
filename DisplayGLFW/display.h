#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <GLFW\glfw3.h>
#include <string>

class Display
{
	bool isFullScreen;
public:
	Display(int width, int height, const std::string& title);

	void Clear(float r, float g, float b, float a);
	void SwapBuffers();
	void PollEvents();
	bool IsFullscreen(  );
	void* getScene();
	void addKeyCallBack(void(*func)(GLFWwindow *,int,int,int,int));
	void addMouseCallBacks(void (* mousebuttonfun)(GLFWwindow*,int,int,int),void(* scrollfun)(GLFWwindow*,double,double),void (* cursorposfun)(GLFWwindow*,double,double));
	void addResizeCallBack(void (*windowsizefun)(GLFWwindow*,int,int));
	
	void setScene(void *userPointer);

	void mouseProccessing(int button);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	//static void mouse_callback(GLFWwindow* window,int button, int action, int mods) ;
	//static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) ;
	//void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	//static void window_size_callback(GLFWwindow* window, int width, int height);

	int closeWindow();
	 ~Display();
//protected:
private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}

	GLFWwindow* m_window;
	int error;
};

#endif
