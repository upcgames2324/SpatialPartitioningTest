#pragma once
#include "Module.h"
#include "Math/float4x4.h"

class ModuleProgram;
class Model;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void LoadPredefinedModel(int model);
	void LoadModel(const std::string& path, const float4x4& modelMatrix);

private:
	unsigned CreateTriangleVBO();
	void RenderVBO(unsigned vbo) const;
	void DestroyVBO(unsigned vbo) const;

	float4x4 LookAt(const float3& position, const float3& target, float3& up) const;

private:
	unsigned program_id;
	ModuleProgram* moduleProgram;
	//std::vector<Model*> models;
	Model* model;
	std::string currentModelPath;
};
