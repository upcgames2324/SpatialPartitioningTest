#include "ModuleTexture.h"
#include "DirectXTex.h"
#include <GL/glew.h>

ModuleTexture::ModuleTexture()
{
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Init()
{
	return true;
}

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp()
{
	glDeleteTextures(1, &texture1);

	return true;
}

void ModuleTexture::LoadTexture(const wchar_t* file)
{
	// auto image = std::make_unique<DirectX::ScratchImage>();
	HRESULT hr = DirectX::LoadFromDDSFile(file,
		DirectX::DDS_FLAGS_NONE, &info, image);
	if (FAILED(hr)) {
		HRESULT hr = DirectX::LoadFromTGAFile(file, &info, image);
		if (FAILED(hr)) {
			HRESULT hr = DirectX::LoadFromWICFile(file, DirectX::WIC_FLAGS_ALL_FRAMES, &info, image);
			if (FAILED(hr)) {
				assert(false && "Could not load texture from file");
			}
		}
	}

	switch (image.GetMetadata().format)
	{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
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

	// TODO: glGenTextures, glBindTexture, glTexParameter, glTexImage2D, glGenerateMipMap, glActiveTexture
	// TODO: save mutiple textures. Clean them when program ends
	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, image.GetMetadata().mipLevels - 1);

	for (size_t i = 0; i < image.GetMetadata().mipLevels; ++i)
	{
		const DirectX::Image* mip = image.GetImage(i, 0, 0);
		glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);
	}

}
