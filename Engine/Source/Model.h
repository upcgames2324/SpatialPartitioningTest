#pragma once

class Mesh;
namespace tinygltf {
	class Model;
}

class Model
{
public:
	void Load(const char* assetFileName);

	void LoadMaterials(const tinygltf::Model& srcModel);

	void Draw(const unsigned programId) const;

private:
	std::vector<Mesh*> meshes;
	std::vector<unsigned> textures;
};
