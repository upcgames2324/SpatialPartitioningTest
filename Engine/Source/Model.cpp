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
#include "ModuleDebugDraw.h"

#include "Math/float4x4.h"

Model::Model()
{
	modelMatrix = float4x4::identity;
}

Model::~Model()
{
	for (Mesh* mesh : meshes) {
		if (mesh != nullptr) {
			delete(mesh);
		}
	}
}

void Model::Load(const std::string& assetFileName, const float4x4& modelMatrix)
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

	this->modelMatrix = modelMatrix;

	for (const auto& gltfMesh : gltfmodel.meshes)
	{
		for (const auto& primitive : gltfMesh.primitives)
		{
			Mesh* mesh = new Mesh;
			mesh->Load(gltfmodel, gltfMesh, primitive);
			meshes.push_back(mesh);
		}
	}
	if (gltfmodel.materials.size() > 0) {
		size_t pathPos = assetFileName.find_last_of('/');
		if (pathPos == std::string::npos) {
			pathPos = assetFileName.find_last_of('\\');
		}
		const std::string filePath = assetFileName.substr(0, pathPos + 1);
		LoadMaterials(gltfmodel, filePath);
	}

}

void Model::LoadMaterials(const tinygltf::Model& srcModel, const std::string& filePath)
{
	for (const auto& srcMaterial : srcModel.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = srcModel.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = srcModel.images[texture.source];

			std::wstring widestr (filePath.begin(), filePath.end());
			widestr += std::wstring(image.uri.begin(), image.uri.end());
			const wchar_t* imageUri = widestr.c_str();
			
			textureId = App->GetModuleTexture()->LoadTexture(imageUri);
		}
		textures.push_back(textureId);
	}
}

void Model::Draw(const unsigned programId) const
{
	for (int i = 0; i < meshes.size(); ++i)
	{
		unsigned textureId = (textures.size() > i) ? textures[i] : 0;
		meshes[i]->Draw(programId, textureId);

		// Call DrawBoundingBox after drawing each mesh
		debugDraw->DrawBoundingBox(meshes[i]->GetBoundingBox());

	}

}

const float4x4 Model::GetModelMatrix() const
{
	return modelMatrix;
}

void Model::SetDebugDraw(ModuleDebugDraw* debugDraw)
{
	this->debugDraw = debugDraw;
}