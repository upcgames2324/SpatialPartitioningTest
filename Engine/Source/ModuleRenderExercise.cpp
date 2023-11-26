#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"
#include "Application.h"
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
	LOG("Creating triangle exercise");
	vbo1 = CreateTriangleVBO();
	App->GetModuleTexture()->LoadTexture(L"../Models/Textures/Test-image-Baboon.ppm");

	// Create basic vertex and fragment shader
	char* vertex_shader = moduleProgram->LoadShaderSource("../Source/shaders/vertex_01_modelview.glsl"); // vertex_hello_world, vertex_01_modelview
	unsigned vertex_id = moduleProgram->CompileShader(GL_VERTEX_SHADER, vertex_shader);

	char* fragment_shader = moduleProgram->LoadShaderSource("../Source/shaders/fragment_hello_world.glsl");
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
	RenderVBO(vbo1);

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
		0.0f, 1.0f, 0.0f,
		
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.5f, 0.0f
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
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f), float4x4::RotateZ(pi / 4.0f), float3(2.0f, 1.0f, 1.0f));
	float4x4 view1 = LookAt(float3(0.0f, 0.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);
	float4x4 view2 = float4x4::LookAt(float3(0.0f, 0.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY, float3::unitY);
	float4x4 view = ComputeProjectionMatrix(App->GetWindow()->GetAspectRatio(), true);
	float4x4 proj = ComputeProjectionMatrix(App->GetWindow()->GetAspectRatio(), false);
	
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
		(void*)(sizeof(float) * 3 * 3) // buffer offset
	);
	//glActiveTexture(GL_TEXTURE5);
	//glBindTexture(GL_TEXTURE_2D, texture_object);
	
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);

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

float4x4 ModuleRenderExercise::ComputeProjectionMatrix(float aspect, bool viewMatrix) {
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3(0.0f, 1.0f, 8.0f);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);

	if (viewMatrix) {
		return frustum.ViewMatrix();
	}
	return frustum.ProjectionMatrix();
}
