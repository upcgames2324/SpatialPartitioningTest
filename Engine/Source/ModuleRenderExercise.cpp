#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "Model.h"
#include "Application.h"
#include "MathGeoLib.h"
#include <GL/glew.h>

ModuleRenderExercise::ModuleRenderExercise()
{
	moduleProgram = new ModuleProgram();
	program_id = 0;
}

ModuleRenderExercise::~ModuleRenderExercise()
{
	delete moduleProgram;
	for (Model* model : models) {
		if (model != nullptr) {
			delete(model);
		}
	}
}

bool ModuleRenderExercise::Init()
{
	LoadPredefinedModel(0);

	// Create program from vertex and fragment shader
	char* vertex_shader = moduleProgram->LoadShaderSource("./Shaders/vertex_02_textures.glsl");
	unsigned vertex_id = moduleProgram->CompileShader(GL_VERTEX_SHADER, vertex_shader);

	char* fragment_shader = moduleProgram->LoadShaderSource("./Shaders/fragment_02_textures.glsl");
	unsigned fragment_id = moduleProgram->CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

	program_id = moduleProgram->CreateProgram(vertex_id, fragment_id);

	return true;


}
update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::Update()
{
	float4x4 view = App->GetModuleCamera()->GetViewMatrix();
	float4x4 proj = App->GetModuleCamera()->GetProjectionMatrix();

	glUseProgram(program_id);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);
	
	for (const Model* model : models)
	{
		float4x4 modelMatrix = model->GetModelMatrix();
		glUniformMatrix4fv(0, 1, GL_TRUE, &modelMatrix[0][0]);
		model->Draw(program_id);
	}
	
	App->GetModuleDebugDraw()->Draw(view, proj, App->GetWindow()->GetWidth(), App->GetWindow()->GetHeight());
	
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	return true;
}

void ModuleRenderExercise::LoadPredefinedModel(int model)
{
	std::string path;
	switch (model)
	{
		case 0:
			path = "./Models/BakerHouse/BakerHouse.gltf";
			if (currentModelPath != path) {
				LoadModel(path, float4x4::FromTRS(float3::zero, float4x4::identity, float3(50.f, 50.f, 50.f)));
			}
		break;
		case 1:
			path = "./Models/Duck/Duck.gltf";
			if (currentModelPath != path) {
				LoadModel(path, float4x4::FromTRS(float3::zero, float4x4::RotateY(-math::pi / 2), float3(.01f, .01f, .01f)));
			}
		break;
		case 2:
			path = "./Models/Avocado/Avocado.gltf";
			if (currentModelPath != path) {
				LoadModel(path, float4x4::FromTRS(float3::zero, float4x4::RotateY(math::pi), float3(20.f, 20.f, 20.f)));
			}
		break;
		case 3:
			path = "./Models/Patricio/Patricio.gltf";
			if (currentModelPath != path) {
				LoadModel(path, float4x4::FromTRS(float3(0.f, 0.5f, 0.f), float4x4::RotateX(-math::pi / 2), float3(.005f, .005f, .005f)));
			}
		break;
		case -1:

		break;
	}
}

void ModuleRenderExercise::LoadModel(const std::string& path)
{
	App->GetModuleEditor()->CustomModelLoaded();
	LoadModel(path, float4x4::identity);
}

void ModuleRenderExercise::LoadModel(const std::string& path, const float4x4& modelMatrix = float4x4::identity)
{
	Model* model1 = new Model();
	model1->Load(path, modelMatrix);
	models.clear();
	models.push_back(model1);

	currentModelPath = path;
}

// This function must be called one time at creation of vertex buffer
unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	glObjectLabel(GL_BUFFER, vbo, -1, "Quad");
	return vbo;
}

// This function must be called each frame for drawing the triangle
void ModuleRenderExercise::RenderVBO(unsigned vbo) const
{
	float4x4 model = float4x4::identity;
	float4x4 view = App->GetModuleCamera()->GetViewMatrix();
	float4x4 proj = App->GetModuleCamera()->GetProjectionMatrix();
	
	//glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, "Render Quad");

	glUseProgram(program_id);
	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// Textures
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void*)(sizeof(float) * 3 * 6) // buffer offset
	);
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, App->GetModuleTexture()->texture1);
	
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Debug drawing
	App->GetModuleDebugDraw()->Draw(view, proj, App->GetWindow()->GetWidth(), App->GetWindow()->GetHeight());

	//glPopDebugGroup();
}

// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo) const
{
	glDeleteBuffers(1, &vbo);
}

float4x4 ModuleRenderExercise::LookAt(const float3& position, const float3& target, float3& up) const
{
	float3 forward = (target - position).Normalized();
	float3 right = forward.Cross(up).Normalized();
	up = right.Cross(forward).Normalized();

	float4x4 viewRotation = float4x4(float3x3(right, up, -forward)).Transposed();
	float4x4 viewTranslation = float4x4(float3x4(float3x3::identity, position));
	return viewRotation * viewTranslation;
}
