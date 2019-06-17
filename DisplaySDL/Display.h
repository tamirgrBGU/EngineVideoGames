#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <SDL2/SDL.h>
#include <string>


struct Window
{
	void *userPointer;
	SDL_Window *window;
};

class Display
{
public:
	Display(int width, int height, const std::string& title);

	void SwapBuffers();
	void PollEvents();
	bool IsFullscreen(  );
	void* getScene();
	void addKeyCallBack(void(*func)(Window *,int,int,int,int));
	void addMouseCallBacks(void (* mousebuttonfun)(Window*,int,int,int),void(* scrollfun)(Window*,double,double),void (* cursorposfun)(Window*,double,double));
	void addResizeCallBack(void (*windowsizefun)(Window*,int,int));
	
	void setScene(void *userPointer);

    inline bool closeWindow() {return m_isclosed;}
	virtual ~Display();
protected:
private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}
	void(*keyFunc)(Window *,int,int,int,int);
	void (* mouseFunc)(Window*,int,int,int);
    void(* scrollFunc)(Window*,double,double);
	void (* cursorPosFunc)(Window*,double,double);
	void (*resizeFunc)(Window*,int,int);

	Window m_window;
	
	SDL_GLContext m_glContext;
	bool m_isclosed;
};

#endif
