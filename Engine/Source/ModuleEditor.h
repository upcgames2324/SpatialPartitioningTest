#pragma once
#include "Module.h"

struct ImGuiContext;

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
	void AddLog(const char* log);

private:
	void ShowUpperMenu() const;
	void ShowWindowConsole() const;

private:
	ImGuiContext* imGuiContext;
	std::vector<const char*> logs;
};
