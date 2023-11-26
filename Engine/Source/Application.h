#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleEditor;
class ModuleDebugDraw;
class ModuleCamera; 
class ModuleTexture;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput* GetInput() { return input; }
    ModuleDebugDraw*  GetModuleDebugDraw() { return debugDraw; }
    ModuleTexture* GetModuleTexture() { return moduleTexture; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleRenderExercise* renderExercise = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleDebugDraw* debugDraw = nullptr;
    ModuleCamera* moduleCamera = nullptr;
    ModuleTexture* moduleTexture = nullptr;

    std::list<Module*> modules;

};

extern Application* App;
