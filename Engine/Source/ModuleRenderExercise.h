#pragma once
#include "Module.h"
#include "MathGeoLib.h"

class ModuleProgram;

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

	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	void RenderVBO(unsigned vbo);

	float4x4 LookAt(float3 position, float3 target, float3 direction);
	float4x4 ComputeProjectionMatrix(float aspect);

private:
	unsigned vbo1, program_id;
	ModuleProgram* moduleProgram;
};

