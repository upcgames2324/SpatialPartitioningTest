#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "tiny_gltf.h"

#include "Model.h"
#include "Mesh.h"
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
		}
	}
}