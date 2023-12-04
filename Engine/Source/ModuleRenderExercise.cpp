#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"
#include "ModuleCamera.h"
#include "Model.h"
#include "Application.h"
#include "MathGeoLib.h"
#include <GL/glew.h>

ModuleRenderExercise::ModuleRenderExercise()
{
	moduleProgram = new ModuleProgram();
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	//LOG("Creating triangle exercise");
	//vbo1 = CreateTriangleVBO();
	//App->GetModuleTexture()->LoadTexture(L"./Textures/Test-image-Baboon.ppm");

	// Loading models
	Model* model1 = new Model();
	model1->Load("./Models/SampleModels/TriangleWithoutIndices.gltf");
	models.push_back(model1);

	// Create basic vertex and fragment shader
	char* vertex_shader = moduleProgram->LoadShaderSource("../Source/shaders/vertex_01_modelview.glsl"); // vertex_hello_world, vertex_01_modelview, vertex_02_textures
	unsigned vertex_id = moduleProgram->CompileShader(GL_VERTEX_SHADER, vertex_shader);

	char* fragment_shader = moduleProgram->LoadShaderSource("../Source/shaders/fragment_hello_world.glsl"); // fragment_hello_world, fragment_02_textures
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
	// Render simple triangle
	//RenderVBO(vbo1);

	// TODO: QUIT
	float4x4 model = float4x4::identity;//float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 1.0f));
	float4x4 view = App->GetModuleCamera()->GetViewMatrix();
	float4x4 proj = App->GetModuleCamera()->GetProjectionMatrix();

	glUseProgram(program_id);
	glUniformMatrix4fv(0, 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(1, 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(2, 1, GL_TRUE, &proj[0][0]);

	App->GetModuleDebugDraw()->Draw(view, proj, App->GetWindow()->GetWidth(), App->GetWindow()->GetHeight());
	//
	
	for (const Model* model : models)
	{
		model->Draw(program_id);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	LOG("Destroying renderer");
	DestroyVBO(vbo1);

	return true;
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
void ModuleRenderExercise::RenderVBO(unsigned vbo)
{
	float4x4 model = float4x4::identity;//float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 1.0f));
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
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

float4x4 ModuleRenderExercise::LookAt(float3 position, float3 target, float3 up) {
	float3 forward = (target - position).Normalized();
	float3 right = forward.Cross(up).Normalized();
	up = right.Cross(forward).Normalized();

	float4x4 viewRotation = float4x4(float3x3(right, up, -forward)).Transposed();
	float4x4 viewTranslation = float4x4(float3x4(float3x3::identity, position));
	return viewRotation * viewTranslation;
}
