#pragma once
#include "Module.h"

class ModuleProgram : public Module
{
public:
	ModuleProgram();
	~ModuleProgram();

	char* LoadShaderSource(const char* shader_file_name) const;
	unsigned CompileShader(unsigned type, const char* source) const;
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader) const;

};

