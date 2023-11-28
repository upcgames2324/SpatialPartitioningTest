#pragma once
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

class Mesh
{
public:
	void Load(tinygltf::Model, tinygltf::Mesh, tinygltf::Primitive);

private:
	// VBO filled from tinygltf primitive attributes
	// EBO filled from tinygltf primitive indices
	// Material index from tinygltf primitive material
	unsigned vbo, ebo, materialIndex;
};

