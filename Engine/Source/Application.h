#pragma once

#include "Globals.h"
#include <vector>

class Module;
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
    ModuleEditor* GetModuleEditor() { return editor; }
    ModuleDebugDraw*  GetModuleDebugDraw() { return debugDraw; }
    ModuleCamera* GetModuleCamera() { return moduleCamera; }
    ModuleTexture* GetModuleTexture() { return moduleTexture; }
    ModuleRenderExercise* GetModuleRender() { return renderExercise; }

private:
    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleRenderExercise* renderExercise = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleDebugDraw* debugDraw = nullptr;
    ModuleCamera* moduleCamera = nullptr;
    ModuleTexture* moduleTexture = nullptr;

    std::vector<Module*> modules;

};

extern Application* App;
