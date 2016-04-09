#include "Light.h"


Light::Light()
{
}


Light::Light(vec3& position, vec3& ambient, vec3 diffuse, vec3 specular) :Position(position), 
AmbienLightColor(ambient), DiffuseLightColor(diffuse), SpecularLightColor(specular)
{
	vector<TexturedModel*> models = Loader::GetInstance()->LoadToFromFile("../Resources/Models/cube.obj");
	model = models[0];

}

Light::~Light()
{
	if (model)
	{
		delete model;
	}
}


vec3& Light::GetPosition()
{
	return Position;
}

void Light::SetPosition(vec3& position)
{
	Position = position;
}


vec3& Light::GetAmbientLightColor()
{
	return AmbienLightColor;
}


void Light::SetAmbientLightColor(vec3& color)
{
	AmbienLightColor = color;
}

vec3& Light::GetDiffuseLightColor()
{
	return DiffuseLightColor;
}

void Light::SetDiffuseLightColor(vec3& color)
{
	DiffuseLightColor = color;
}

vec3& Light::GetSpecularLightColor()
{
	return SpecularLightColor;
}


void Light::SetSpecularLightColor(vec3& color)
{
	SpecularLightColor = color;
}

void Light::Draw(Shader shader)
{
	model->Draw();
}

TexturedModel* Light::GetModel()
{
	return model;
}