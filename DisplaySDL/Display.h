	#ifndef DISPLAY_INCLUDED_H
#define DISPLAY_INCLUDED_H

#include <string>
#include <SDL2/SDL.h>

class Display
{
public:
	Display(int width, int height, const std::string& title);
	Display(int width, int height, const std::string& title);

	void SwapBuffers();
	void PollEvents();
	bool IsFullscreen(  );
	void* getScene();
	void addKeyCallBack(void(*func)(SDL_Window *,int,int,int,int));
	void addMouseCallBacks(void (* mousebuttonfun)(SDL_Window*,int,int,int),void(* scrollfun)(SDL_Window*,double,double),void (* cursorposfun)(SDL_Window*,double,double));
	void addResizeCallBack(void (*windowsizefun)(SDL_Window*,int,int));
	
	void setScene(void *userPointer);

    inline bool closeWindow() {return m_isclosed;}
	virtual ~Display();
protected:
private:
	void operator=(const Display& display) {}
	Display(const Display& display) {}
	void(*keyFunc)(SDL_Window *,int,int,int,int);
	void (* mouseFunc)(SDL_Window*,int,int,int);
    void(* scrollFunc)(SDL_Window*,double,double);
	void (* cursorPosFunc)(SDL_Window*,double,double);
	void (*resizeFunc)(SDL_Window*,int,int);

	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	bool m_isclosed;
};

#endif
