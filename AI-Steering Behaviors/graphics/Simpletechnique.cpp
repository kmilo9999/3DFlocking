#include "Simpletechnique.h"


Simpletechnique::Simpletechnique()
{
	LoadShaders();
}


Simpletechnique::~Simpletechnique()
{
}


void Simpletechnique::LoadShaders()
{

	shader.CreateProgram();
	shader.CreateShaderFromFile("Shaders/SimpleVertexShader.glsl", ShaderType::VERTEX_SHADER);
	shader.CreateShaderFromFile("Shaders/SimpleFragmentShader.glsl", ShaderType::FRAGMENT_SHADER);
	shader.LinkProgram();

	if (!shader.IsLinked())
		throw runtime_error("Shader not Linked Properly");

	shader.BindAttributeLocation(0, "Position");
	shader.BindAttributeLocation(1, "UVs");
}


void Simpletechnique::StartTechnique()
{
	shader.Use();
}

void Simpletechnique::PrepareShadingTechnique(mat4 ProjectionMatrix, mat4 viewMatrix, mat4 ModelMatrix)
{
	
	shader.SetUniformLocation("projectionMatrix", ProjectionMatrix);
	shader.SetUniformLocation("viewMatrix", viewMatrix);

	//mat4 tranformation = Math::CreateTransformationMatrix(light->GetPosition(), quat(1.0f, 0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f));
	shader.SetUniformLocation("transformationMatrix", ModelMatrix);
}

 void Simpletechnique::LoadVariables(map<string, mytypes> variables)
{

}

 void Simpletechnique::StopTechnique()
{
	shader.Unuse();
}