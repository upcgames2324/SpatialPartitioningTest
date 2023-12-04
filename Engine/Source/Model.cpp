#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "tiny_gltf.h"

#include "Model.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Globals.h"

void Model::Load(const char* assetFileName)
{
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model gltfmodel;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&gltfmodel, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
		return;
	}

	for (const auto& gltfMesh : gltfmodel.meshes)
	{
		for (const auto& primitive : gltfMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->Load(gltfmodel, gltfMesh, primitive);
			meshes.push_back(mesh);
		}
	}

	LoadMaterials(gltfmodel);
}

void Model::LoadMaterials(const tinygltf::Model& srcModel)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];
			const wchar_t* imageUri = std::wstring(image.uri.begin(), image.uri.end()).c_str();
			textureId = App->GetModuleTexture()->LoadTexture(imageUri);
		}
		textures.push_back(textureId);
	}
}

void Model::Draw(const unsigned programId) const
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		unsigned textureId = (textures.size() > i) ? i : 0;
		meshes[i]->Draw(programId, textureId);
	}
}
