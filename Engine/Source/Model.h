#pragma once

class Mesh;
namespace tinygltf {
	class Model;
}

class Model
{
public:
	~Model();

	void Load(const std::string& assetFileName);

	void LoadMaterials(const tinygltf::Model& srcModel, const std::string& filePath);

	void Draw(const unsigned programId) const;

private:
	std::vector<Mesh*> meshes;
	std::vector<unsigned> textures;
};
