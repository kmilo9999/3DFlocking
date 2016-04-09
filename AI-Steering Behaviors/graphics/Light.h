#pragma once
#include "Common.hpp"
#include "TexturedModel.h"
#include "Loader.h"
#include "Shader.h"

class Light
{
public:
	Light();
	Light(vec3& position,vec3& ambient,vec3 diffuse, vec3 specular);
	~Light();
	vec3& GetPosition();
	void SetPosition(vec3& position);


	TexturedModel* GetModel();

	vec3& GetAmbientLightColor();
	void SetAmbientLightColor(vec3& color);

	vec3& GetDiffuseLightColor();
	void SetDiffuseLightColor(vec3& color);

	vec3& GetSpecularLightColor();
	void SetSpecularLightColor(vec3& color);

	void Draw(Shader shader);

private:
	TexturedModel* model;

	vec3 Position;
	vec3 AmbienLightColor;
	vec3 DiffuseLightColor;
	vec3 SpecularLightColor;
};

