#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init(JSON_Object* object);
	bool CleanUp();

	void GetSize(int& width, int& height);
	void SetSize(int width, int height);

	float GetBrightness();
	void SetBrightness(float brightness);

	void SetTitle(const char* title);
	void SetFullscreen(bool setFullscreen);
	void SetFullscreenDesktop(bool setFullscreenDesktop);
	void SetResizable(bool setResizable);
	void SetBorderless(bool setBorderless);

private:
	int width;
	int height;

	float brightness;

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

	SDL_GLContext context;

	bool fullscreen;
	bool fullscreen_desktop;
	bool resizable;
	bool borderless;
};

#endif // __ModuleWindow_H__