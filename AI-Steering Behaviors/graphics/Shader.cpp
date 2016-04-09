#include "Shader.h"
#include <fstream>
#include <stdio.h>

Shader::Shader(void):program(0),log(""), isLinked(false)
{
}


Shader::~Shader(void)
{
}


void Shader::CreateProgram()
{
	program = glCreateProgram();
}

bool Shader::CreateShaderFromFile(const char* fileName, ShaderType type)
{
	// Read the source from the named file
	char* src = ReadFile(fileName);
	const char* psrc[1] = { src };

	// Create a shader and attach, hand it the source, and compile it.
	GLint shader;
	if (type == ShaderType::VERTEX_SHADER)
	{
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == ShaderType::FRAGMENT_SHADER)
	{
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	glAttachShader(program, shader);
	glShaderSource(shader, 1, psrc, NULL);
	glCompileShader(shader);
	delete src;

	// Get the compilation status
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	// If compilation status is not OK, get and print the log message.
	if (status != 1) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* buffer = new char[length];
		glGetShaderInfoLog(shader, length, NULL, buffer);
		printf("Compile log for %s:\n%s\n", fileName, buffer);
		delete buffer;
	}
	else {
		printf("Compile log for %s: %s\n", fileName,"successfully compiled");
	}

	
	return true;
}

void Shader::CreateShaderFromMemory(const char* source, ShaderType type)
{
	 
	GLint shader;
	if (type == ShaderType::VERTEX_SHADER)
	{
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == ShaderType::FRAGMENT_SHADER)
	{
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}

	glAttachShader(program, shader);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	

	// Get the compilation status
	int status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	// If compilation status is not OK, get and print the log message.
	if (status != 1) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* buffer = new char[length];
		glGetShaderInfoLog(shader, length, NULL, buffer);
		printf("Compile log for source:\n%s\n", buffer);
		delete buffer;
		
	}
	
	
	
}


void Shader::LinkProgram()
{
	// Link program and check the status
	if (!isLinked)
	{
		glLinkProgram(program);
		int status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);

		// If link failed, get and print log
		if (status != 1) {
			int length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			char* buffer = new char[length];
			glGetProgramInfoLog(program, length, NULL, buffer);
			printf("Link log:\n%s\n", buffer);
			delete buffer;
			isLinked = false;
			glDeleteProgram(program);
		}

	    isLinked = true;
	}
	
	
}

bool Shader::IsLinked()
{
	return isLinked;
}

void Shader::Use()
{
	if(!InUse())
	{
		glUseProgram(program);
	}
	
}

bool Shader::InUse()
{
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

	return currentProgram == (GLint)program;
}

void Shader::Unuse()
{
	if (InUse())
	{
		glUseProgram(0);
	}
	
}

char* Shader::ReadFile(const char* name)
{
	ifstream f;
	f.open(name, ios_base::binary); // Open
	f.seekg(0, ios_base::end);      // Position at end
	auto size = f.tellg();
	int length = (int)size;      //   to get the length

	char* content = new char[length + 1]; // Create buffer of needed length
	f.seekg(0, ios_base::beg);     // Position at beginning
	f.read(content, length);            //   to read complete file
	f.close();                           // Close

	content[length] = char(0);           // Finish with a NULL
	return content;
}

void Shader::BindAttributeLocation( GLuint location,  string name)
{
	glBindAttribLocation(program, location, name.c_str());
	AttributeLocations[name] = location;
}

GLint Shader::GetAttributeLocation(const string name)
{
	if (!InUse())
		Use();
	auto found = AttributeLocations.find(name);
	if (found != AttributeLocations.end())
	{		
		return found->second;
	}
	else{
		GLint location =  glGetAttribLocation(program,name.c_str());
		AttributeLocations[name]  = location;
		return location;
	}
}

GLint Shader::GetUniformLocation(const string name)
{
	if (!InUse())
		Use();
	auto found = AttributeLocations.find(name);
	if (found != AttributeLocations.end())
	{
		return found->second;
	}
	GLint location = glGetUniformLocation(program, name.c_str());
	UniformLocations[name] = location;
	return location;
}

void Shader::SetUniformLocation(const string name, float x, float y, float z, float w)
{
	if (!InUse())
		Use();
	GLint location = GetUniformLocation(name);
	if(location != -1)
		glUniform4f(location, x, y, z,w);
}

void Shader::SetUniformLocation(const string name, float x, float y, float z)
{
	if (!InUse())
		Use();
	GLint location = GetUniformLocation( name);
	if (location != -1)
		glUniform3f(location, x,y,z);
}

void Shader::SetUniformLocation(const string name, float x, float y)
{
	if (!InUse())
		Use();
	GLint location = GetUniformLocation(name);
	if (location != -1)
		glUniform2f(location, x, y);
}

void Shader::SetUniformLocation(const string name, float x)
{
	if (InUse())
		Use();
	GLint location = GetUniformLocation(name);
	if (location != -1)
		glUniform1f(location, x);
}

void Shader::SetUniformLocation(const string name, int i)
{
	if (!InUse())
		Use();
	GLint location = GetUniformLocation(name);
	if (location != -1)
	glUniform1i(location, i);
}

void Shader::SetUniformLocation(const string name, const vec2 &vector)
{
	if (!InUse())
		Use();

	GLint location = GetUniformLocation(name);
	if (location != -1)
	glUniform2fv(location, 1, &vector[0]);
}

void Shader::SetUniformLocation(const string name, const vec3 &vector)
{

	if (!InUse())
		Use();
	GLint location = GetUniformLocation(name);
	if (location != -1)
		glUniform3fv(location, 1, &vector[0]);
}

void Shader::SetUniformLocation(const string name, const vec4 &vector)
{
	if (!InUse())
		Use();
	GLint location = GetUniformLocation(name);
	if (location != -1)
		glUniform4fv(location, 1, &vector[0]);
}

void Shader::SetUniformLocation(const string name, const  mat4 &matrix)
{

	if (!InUse())
		Use();
	GLint location = GetUniformLocation(name);
	if (location != -1)
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}


const string Shader::GetErrorLog()
{
	return log;
}

