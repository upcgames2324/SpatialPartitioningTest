#pragma once
#include "Module.h"

#define VECTOR_SIZE 128

struct ImGuiContext;
typedef int ImGuiWindowFlags;
typedef uint64_t Uint64;

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
	void ShowWindowConsole(bool& show_window_console) const;
	void ShowWindowPerformance(bool& show_window_performance) const;
	void ShowWindowSystemInfo(bool& show_window_system_info) const;
	void ShowHelpAbout(bool& show_help_about) const;


private:
	ImGuiContext* imGuiContext;
	ImGuiWindowFlags windowFlags;
	std::vector<const char*> logs;
	std::vector<float> elapsedTime, fpsLog;
	Uint64 timer;
};
