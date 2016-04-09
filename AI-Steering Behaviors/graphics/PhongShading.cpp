#include "PhongShading.h"


PhongShading::PhongShading(Light* globalLight)
{
	LoadShaders();
	GlobalLight = globalLight;
}


PhongShading::~PhongShading()
{
}

void PhongShading::LoadShaders()
{
	Shader.CreateProgram();
	Shader.CreateShaderFromFile("Shaders/MeshVertexShader.glsl", ShaderType::VERTEX_SHADER);
	Shader.CreateShaderFromFile("Shaders/MeshFragmentShader.glsl", ShaderType::FRAGMENT_SHADER);
	Shader.LinkProgram();

	if (!Shader.IsLinked())
		throw runtime_error("Shader not Linked Properly");

	Shader.BindAttributeLocation(0, "Position");
	Shader.BindAttributeLocation(1, "UVs");
	Shader.BindAttributeLocation(2, "Normal");
}

void PhongShading::StartTechnique()
{
	Shader.Use();
}

void PhongShading::LoadVariables(map<string, mytypes> variables)
{
	


	Shader.SetUniformLocation("ambient", variables["ambient"].vector4);
	Shader.SetUniformLocation("diffuse", variables["diffuse"].vector4);
	Shader.SetUniformLocation("specular", variables["specular"].vector4);
	Shader.SetUniformLocation("shininess", variables["shininess"].floating);
	bool isTextured = variables["isTextured"].boolean;

	Shader.SetUniformLocation("isTextured", isTextured);
	if (isTextured)
	{
		Shader.SetUniformLocation("diffuseMap", variables["diffuseMap"].integer);
		Shader.SetUniformLocation("specularMap", variables["specularMap"].integer);
	}
	
	Shader.SetUniformLocation("hasDiffusMap", variables["hasDiffusMap"].boolean);
	Shader.SetUniformLocation("hasSpecularMap", variables["hasSpecularMap"].boolean);


	Shader.SetUniformLocation("lightPos", GlobalLight->GetPosition());
	Shader.SetUniformLocation("light.ambient", GlobalLight->GetAmbientLightColor());
	Shader.SetUniformLocation("light.diffuse", GlobalLight->GetDiffuseLightColor());
	Shader.SetUniformLocation("light.specular", GlobalLight->GetSpecularLightColor());

}

void PhongShading::PrepareShadingTechnique(mat4 ProjectionMatrix, mat4 viewMatrix, mat4 ModelMatrix)
{
	Shader.SetUniformLocation("projectionMatrix", ProjectionMatrix);
	Shader.SetUniformLocation("viewMatrix", viewMatrix);

	//mat4 tranformation = Math::CreateTransformationMatrix(entity->GetPosition(), entity->GetOrientation(), entity->GetScale());
	Shader.SetUniformLocation("transformationMatrix", ModelMatrix);
}

void PhongShading::StopTechnique()
{
	Shader.Unuse();
}
