#pragma once

#include "OpenglGL.h"
#include <string>
#include <map>
#include "Common.hpp"
#include <vector>

enum class ShaderType
{
	VERTEX_SHADER,
	FRAGMENT_SHADER
};



class Shader
{
public:
	Shader(void);
	~Shader(void);
	
	static const int MAX_BONES = 100;

	void CreateProgram();
	bool CreateShaderFromFile(const char* fileName, ShaderType type);
	void CreateShaderFromMemory(const char* source, ShaderType type);
	void LinkProgram();
	bool IsLinked();
	void Use();
	void Unuse();
	bool InUse();

	GLint GetUniformLocation(const string name);
	void SetUniformLocation(const string name, float x, float y, float z, float w);
	void SetUniformLocation(const string name, float x, float y, float z);
	void SetUniformLocation(const string name, float x, float y);
	void SetUniformLocation(const string name, float x);
	void SetUniformLocation(const string name, int i);
	void SetUniformLocation(const string name, const vec2 &vector);
	void SetUniformLocation(const string name, const vec3&vector);
	void SetUniformLocation(const string name, const vec4 &vector);
	void SetUniformLocation(const string name, const mat4 &matrix);
	

	GLuint GetProgramID() { return program; };
	const string GetErrorLog();

	void BindAttributeLocation( GLuint location,  string name);
	GLint GetAttributeLocation(const string name);




private:
	char* ReadFile(const char* name);
	GLuint program;
	string log;
	bool isLinked;
	map<string, GLint> UniformLocations;
	map<string, GLint> AttributeLocations;
	
};

