#include "TerrainShader.h"


TerrainShader::TerrainShader(Light* light, float scaleuv) :ScaleUV(scaleuv)
{
	LoadShaders();
	GlobalLight = light;
}


TerrainShader::~TerrainShader()
{
}

void TerrainShader::LoadShaders()
{
	Shader.CreateProgram();
	Shader.CreateShaderFromFile("Shaders/TerrainVertexShader.glsl", ShaderType::VERTEX_SHADER);  
	Shader.CreateShaderFromFile("Shaders/TerrainFragmentShader.glsl", ShaderType::FRAGMENT_SHADER);
	Shader.LinkProgram();

	if (!Shader.IsLinked())
		throw runtime_error("Shader not Linked Properly");

	Shader.BindAttributeLocation(0, "Position");
	Shader.BindAttributeLocation(1, "UVs");
	Shader.BindAttributeLocation(2, "Normal");
}

void TerrainShader::StartTechnique()
{
	Shader.Use();
}

void TerrainShader::LoadVariables(map<string, mytypes> variables)
{
	Shader.SetUniformLocation("ambient", variables["ambient"].vector4);
	Shader.SetUniformLocation("diffuse", variables["diffuse"].vector4);
	Shader.SetUniformLocation("specular", vec4(0.76f, 0.76f, 0.76f,1.0f));
	Shader.SetUniformLocation("shininess", variables["shininess"].floating);
	bool isTextured = variables["isTextured"].boolean;

	Shader.SetUniformLocation("isTextured", isTextured);
	if (isTextured)
	{
		Shader.SetUniformLocation("diffuseMap", variables["diffuseMap"].integer);
	}

	Shader.SetUniformLocation("hasDiffusMap", variables["hasDiffusMap"].boolean);
	Shader.SetUniformLocation("hasSpecularMap", variables["hasSpecularMap"].boolean);


	Shader.SetUniformLocation("lightPos", GlobalLight->GetPosition());
	Shader.SetUniformLocation("light.ambient", GlobalLight->GetAmbientLightColor());
	Shader.SetUniformLocation("light.diffuse", GlobalLight->GetDiffuseLightColor());
	Shader.SetUniformLocation("light.specular", GlobalLight->GetSpecularLightColor());
	
	Shader.SetUniformLocation("scaleUV", ScaleUV);
}

void TerrainShader::PrepareShadingTechnique(mat4 ProjectionMatrix, mat4 viewMatrix, mat4 ModelMatrix)
{
	Shader.SetUniformLocation("projectionMatrix", ProjectionMatrix);
	Shader.SetUniformLocation("viewMatrix", viewMatrix);

	//mat4 tranformation = Math::CreateTransformationMatrix(entity->GetPosition(), entity->GetOrientation(), entity->GetScale());
	Shader.SetUniformLocation("transformationMatrix", ModelMatrix);
}

void TerrainShader::StopTechnique()
{
	Shader.Unuse();
}
