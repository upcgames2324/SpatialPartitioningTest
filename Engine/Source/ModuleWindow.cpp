#include "Globals.h"
#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include <GL/glew.h>

ModuleWindow::ModuleWindow()
{
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		int width = SCREEN_WIDTH;
		int height = SCREEN_HEIGHT;
		Uint32 flags = SDL_WINDOW_SHOWN |  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		if(FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

update_status ModuleWindow::PreUpdate()
{
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

int ModuleWindow::GetWidth() const
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	return DM.w;
}

int ModuleWindow::GetHeight() const
{
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	return DM.h;
}

float ModuleWindow::GetAspectRatio() const {
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);
	return (float)DM.w / (float)DM.h;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::WindowResized(unsigned width, unsigned height)
{
	glViewport(0, 0, width, height);
	SDL_SetWindowSize(window, width, height);
	App->GetModuleCamera()->SetAspectRatio((float)width / (float)height);
}