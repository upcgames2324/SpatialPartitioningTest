#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"

class Application;
struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	update_status PreUpdate();
	bool CleanUp();

	int GetWidth() const;
	int GetHeight() const;
	float GetAspectRatio() const;

	void WindowResized(unsigned width, unsigned height);

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;
};

#endif // __ModuleWindow_H__