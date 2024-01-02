#pragma once
#include "Geometry/OBB.h"


class Mesh
{
public:
	Mesh();
	~Mesh();

	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void Draw(const unsigned programId, const unsigned textureId) const;

	const OBB& GetBoundingBox() const { return boundingBox;}

private:
	void CreateVAO();

	void CreateVBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void CreateEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);

	void RenderInterleaved() const;

	void RenderSeparated() const;

private:
	// VBO filled from tinygltf primitive attributes
	// EBO filled from tinygltf primitive indices
	// Material index from tinygltf primitive material
	unsigned vao, vbo, ebo;
	size_t vertexCount, indexCount;
	unsigned posStride, textureStride, normalStride, bufferSize;
	const unsigned char *bufferPos, *bufferTexture, *bufferNormal;
	OBB boundingBox;
};

