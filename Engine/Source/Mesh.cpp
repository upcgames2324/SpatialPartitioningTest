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
	//RenderSeparated();
	RenderInterleaved();

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
		bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);
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
		bufferNormal = &(normalBuffer.data[normalAcc.byteOffset + normalView.byteOffset]);
		bufferSize += sizeof(float) * 3;
	}
	if (itTexture != primitive.attributes.end())
	{
		const tinygltf::Accessor& textureAcc = model.accessors[itTexture->second];
		SDL_assert(textureAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(textureAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& textureView = model.bufferViews[textureAcc.bufferView];
		const tinygltf::Buffer& textureBuffer = model.buffers[textureView.buffer];

		bufferTexture = &(textureBuffer.data[textureAcc.byteOffset + textureView.byteOffset]);
		bufferSize += sizeof(float) * 2;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bufferSize * vertexCount, nullptr, GL_STATIC_DRAW);
	float* ptr = reinterpret_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
	int overhead = 0;
	for (size_t i = 0; i < vertexCount; ++i)
	{
		for (int j = 0; j < 3; ++j) {
			ptr[overhead] = reinterpret_cast<const float*>(bufferPos)[j];
			overhead++;
		}
		bufferPos += (posStride == 0) ? sizeof(float) * 3 : posStride;

		if (bufferNormal != nullptr)
		{
			for (int j = 0; j < 3; ++j) {
				ptr[overhead] = reinterpret_cast<const float*>(bufferNormal)[j];
				overhead++;
			}
			bufferNormal += (normalStride == 0) ? sizeof(float) * 3 : normalStride;
		}

		if (bufferTexture != nullptr)
		{
			for (int j = 0; j < 2; ++j) {
				ptr[overhead] = reinterpret_cast<const float*>(bufferTexture)[j];
				overhead++;
			}
			bufferTexture += (textureStride == 0) ? sizeof(float) * 2 : textureStride;
		}
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, bufferSize, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, bufferSize, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, bufferSize, (void*)(sizeof(float) * 6));
	//glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Mesh::RenderSeparated() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * vertexCount));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 6 * vertexCount));
	//glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
