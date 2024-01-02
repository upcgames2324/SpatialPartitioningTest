#pragma once
#include "Math/float4x4.h"

class Mesh;
class ModuleDebugDraw;

namespace tinygltf {
	class Model;
}

class Model
{
public:
	Model();
	~Model();

	void Load(const std::string& assetFileName, const float4x4& modelMatrix);

	void LoadMaterials(const tinygltf::Model& srcModel, const std::string& filePath);

	void Draw(const unsigned programId) const;
	void SetDebugDraw(ModuleDebugDraw* debugDraw);

	const float4x4 GetModelMatrix() const;

private:
	std::vector<Mesh*> meshes;
	std::vector<unsigned> textures;
	float4x4 modelMatrix;
	ModuleDebugDraw* debugDraw; 
};
