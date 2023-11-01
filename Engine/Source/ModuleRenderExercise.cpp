#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
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

	// Create basic vertex and fragment shader
	char* vertex_shader = moduleProgram->LoadShaderFile("D:\\Repositories\\3D-Engine-Glew\\Engine\\Source\\shaders\\vertex_hello_world.glsl");
	unsigned vertex_id = moduleProgram->CompileShader(GL_VERTEX_SHADER, vertex_shader);

	char* fragment_shader = moduleProgram->LoadShaderFile("D:\\Repositories\\3D-Engine-Glew\\Engine\\Source\\shaders\\fragment_hello_world.glsl");
	unsigned fragment_id = moduleProgram->CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

	unsigned program_id = moduleProgram->CreateProgram(vertex_id, fragment_id);

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
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);
	return vbo;
}

// This function must be called each frame for drawing the triangle
void ModuleRenderExercise::RenderVBO(unsigned vbo)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}
