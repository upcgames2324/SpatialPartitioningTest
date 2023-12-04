#pragma once

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Load(tinygltf::Model model, tinygltf::Mesh mesh, tinygltf::Primitive primitive);

	void RenderInterleaved(const unsigned programId) const;

	void RenderSeparated(const unsigned programId) const;

	void CreateVAO(bool interleaved);

	void Draw(const unsigned programId, const unsigned textureId) const;

private:
	// VBO filled from tinygltf primitive attributes
	// EBO filled from tinygltf primitive indices
	// Material index from tinygltf primitive material
	unsigned vao, vbo, ebo;
	size_t vertexCount, indexCount;
	bool interleavedVertices;
};

