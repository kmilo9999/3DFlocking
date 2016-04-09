#pragma once
#include "ShadingTechnique.h"
#include "Shader.h"
#include "Light.h"

class PhongShading: public ShadingTechnique
{
public:
	PhongShading(Light* globalLight);
	~PhongShading();

	void LoadShaders() override;
	void StartTechnique() override;
	void LoadVariables(map<string, mytypes> variables) override;
	void PrepareShadingTechnique(mat4 ProjectionMatrix, mat4 viewMatrix, mat4 ModelMatrix) override;
	void StopTechnique() override;

private:
	Shader Shader;
	Light* GlobalLight;
};

