#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include <SDL.h>
#include <imgui_impl_sdl2.h>
#include <string>

ModuleInput::ModuleInput()
{
    keyboard = new KeyState[MAX_KEYS];
    memset(keyboard, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
    memset(mouse_buttons, KEY_IDLE, sizeof(KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
    RELEASE_ARRAY(keyboard);
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate()
{
	static SDL_Event event;

	mouse_motion = { 0, 0 };
	//memset(windowEvents, false, WE_COUNT * sizeof(bool));

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}
	while(SDL_PollEvent(&event) != 0)
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch(event.type)
		{
			case SDL_QUIT:
				return UPDATE_STOP;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
						// TODO
						//windowEvents[WE_HIDE] = true;
					break;

					case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
						// TODO
						//windowEvents[WE_SHOW] = true;
					break;

					case SDL_WINDOWEVENT_RESIZED:
					//case SDL_WINDOWEVENT_SIZE_CHANGED:
						App->GetWindow()->WindowResized(event.window.data1, event.window.data2);
					break;
					case SDL_WINDOWEVENT_CLOSE:
						return UPDATE_STOP;
					break;
				}
			break;

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

			case SDL_MOUSEMOTION:
				mouse_motion.X = event.motion.xrel / SCREEN_SIZE;
				mouse_motion.Y = event.motion.yrel / SCREEN_SIZE;
				mouse.X = event.motion.x / SCREEN_SIZE;
				mouse.Y = event.motion.y / SCREEN_SIZE;
			break;

			case SDL_DROPFILE:
				std::string dropFile = event.drop.file;
			break;
		}
	}
	/*
	if(GetWindowEvent(EventWindow::WE_QUIT) == true || GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return UPDATE_STOP;
	*/

    return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update()
{
    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

KeyState ModuleInput::GetKey(int id) const
{
    return keyboard[id];
}

KeyState ModuleInput::GetMouseButtonDown(int id) const
{
    return mouse_buttons[id - 1];
}

const COORD& ModuleInput::GetMouseMotion() const
{
    return mouse_motion;
}

const COORD& ModuleInput::GetMousePosition() const
{
    return mouse;
}
