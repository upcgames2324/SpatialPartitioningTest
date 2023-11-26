#pragma once
#include "Module.h"
#include "DirectXTex.h"

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void LoadTexture(const wchar_t* file);

private:
	DirectX::TexMetadata info;
	DirectX::ScratchImage image;

	int internalFormat, format, type;
	unsigned texture1;
};
