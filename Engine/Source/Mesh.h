#pragma once

class Mesh
{
public:
	void Load(tinygltf::Model model, tinygltf::Mesh mesh, tinygltf::Primitive primitive);

	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void RenderInterleaved() const;

	void RenderSeparated() const;

	void CreateVAO();

	void Draw(const std::vector<unsigned>& textures) const;

private:
	// VBO filled from tinygltf primitive attributes
	// EBO filled from tinygltf primitive indices
	// Material index from tinygltf primitive material
	unsigned programId, vao, vbo, ebo, materialIndex;
	size_t vertexCount, indexCount;
};

