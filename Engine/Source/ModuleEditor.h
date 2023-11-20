#pragma once
#include "Module.h"
#include <imgui.h>

class ImGuiContext;

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	ImGuiContext* imGuiContext;
};

