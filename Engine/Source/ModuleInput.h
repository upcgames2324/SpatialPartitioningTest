#pragma once
#include "Module.h"
#include "Globals.h"

#define MAX_KEYS 300
#define NUM_MOUSE_BUTTONS 5

typedef unsigned __int8 Uint8;

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	KeyState GetKey(int id) const;
	KeyState GetMouseButtonDown(int id) const;
	// Get mouse / axis position
	const COORD& GetMouseMotion() const;
	const COORD& GetMousePosition() const;

private:
	KeyState* keyboard;
	KeyState mouse_buttons[NUM_MOUSE_BUTTONS];
	COORD mouse_motion;
	COORD mouse;
};