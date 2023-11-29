#pragma once

class Mesh;

class Model
{
public:
	void Load(const char* assetFileName);

	void LoadMaterials(const tinygltf::Model& srcModel);

private:
	std::vector<Mesh*> meshes;
	std::vector<unsigned> textures;
};

