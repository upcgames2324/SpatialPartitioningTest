#include "ModuleTexture.h"
#include "DirectXTex.h"

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	return false;
}

update_status ModuleTexture::PreUpdate()
{
	return update_status();
}

update_status ModuleTexture::Update()
{
	return update_status();
}

update_status ModuleTexture::PostUpdate()
{
	return update_status();
}

bool ModuleTexture::CleanUp()
{
	return false;
}

void ModuleTexture::LoadTexture(string& path)
{
	// DirectX::ScratchImage scratchImage
	DirectX::LoadFromDDSFile()//, LoadFromTGAFile, LoadFromWICFile

	/*
	switch (metadata.format)
	{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			Break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			internalFormat = GL_RGBA8;
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
			internalFormat = GL_RGB8;
			format = GL_BGR;
			type = GL_UNSIGNED_BYTE;
			break;
		default:
			assert(false && "Unsupported format");
	}
	*/
}
