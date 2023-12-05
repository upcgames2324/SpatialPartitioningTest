#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

#include "Mesh.h"
#include "MathGeoLib.h"
#include <GL/glew.h>
#include <SDL_assert.h>

Mesh::Mesh()
{
	vao = vbo = ebo = 0;
	vertexCount = indexCount = 0;
	posStride = textureStride = normalStride = bufferSize = 0;
	bufferPos = bufferTexture = bufferNormal = nullptr;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}

void Mesh::Load(tinygltf::Model model, tinygltf::Mesh mesh, tinygltf::Primitive primitive)
{
	CreateVAO();
	CreateVBO(model, mesh, primitive);
	CreateEBO(model, mesh, primitive);

	RenderSeparated();
}

void Mesh::Draw(const unsigned programId, const unsigned textureId) const
{
	glUseProgram(programId);
	glBindVertexArray(vao);
	RenderSeparated();

	if (textureId != 0) // TODO: check condition existent texture
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	//glUniform1i(glGetUniformLocation(programId, "diffuse"), 0);
	if (indexCount > 0)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void Mesh::CreateVBO(tinygltf::Model model, tinygltf::Mesh mesh, tinygltf::Primitive primitive)
{
	const auto& itPos = primitive.attributes.find("POSITION");
	const auto& itNormal = primitive.attributes.find("NORMAL");
	const auto& itTangent = primitive.attributes.find("TANGENT");
	const auto& itTexture = primitive.attributes.find("TEXCOORD_0");

	if (itPos != primitive.attributes.end())
	{
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];

		vertexCount = posAcc.count;
		posStride = posView.byteStride;
		bufferPos = reinterpret_cast<const float*>(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);
		bufferSize += sizeof(float) * 3;
	}
	if (itNormal != primitive.attributes.end())
	{
		const tinygltf::Accessor& normalAcc = model.accessors[itNormal->second];
		SDL_assert(normalAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(normalAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& normalView = model.bufferViews[normalAcc.bufferView];
		const tinygltf::Buffer& normalBuffer = model.buffers[normalView.buffer];

		normalStride = normalView.byteStride;
		bufferNormal = reinterpret_cast<const float*>(normalBuffer.data[normalAcc.byteOffset + normalView.byteOffset]);
		bufferSize += sizeof(float) * 3;
	}
	if (itTexture != primitive.attributes.end())
	{
		const tinygltf::Accessor& textureAcc = model.accessors[itTexture->second];
		SDL_assert(textureAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(textureAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& textureView = model.bufferViews[textureAcc.bufferView];
		const tinygltf::Buffer& textureBuffer = model.buffers[textureView.buffer];

		bufferTexture = reinterpret_cast<const float*>(textureBuffer.data[textureAcc.byteOffset + textureView.byteOffset]);
		bufferSize += sizeof(float) * 2;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, bufferPos, GL_STATIC_DRAW); // No byte stride
	glBufferData(GL_ARRAY_BUFFER, bufferSize * vertexCount, nullptr, GL_STATIC_DRAW);
	float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	for (size_t i = 0; i < vertexCount; ++i)
	{
		ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
		bufferPos += posStride;
	}

	glUnmapBuffer(GL_ARRAY_BUFFER);
}

void Mesh::CreateEBO(tinygltf::Model model, tinygltf::Mesh mesh, tinygltf::Primitive primitive)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	if (primitive.indices >= 0)
	{
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset]);
		indexCount = indAcc.count;
		
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT)
		{
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT)
		{
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (uint16_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE)
		{
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (uint8_t i = 0; i < indexCount; ++i) ptr[i] = bufferInd[i];
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}

void Mesh::RenderInterleaved() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 3 + sizeof(float) * 2, (void*)(sizeof(float) * 3));
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Mesh::RenderSeparated() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferSize, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, bufferSize, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, bufferSize, (void*)(sizeof(float) * 6));
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
