#pragma once
#include "Module.h"

class ModuleTexture : public Module
{
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void LoadTexture(string& path);

private:

};

