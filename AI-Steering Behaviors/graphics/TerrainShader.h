#pragma once
#include "ShadingTechnique.h"
#include "Shader.h"
#include "Light.h"

class TerrainShader : public ShadingTechnique
{
public:
	TerrainShader(Light* globalLight, float scaleuv);
	~TerrainShader();

	void LoadShaders() override;
	void StartTechnique() override;
	void LoadVariables(map<string, mytypes> variables) override;
	void PrepareShadingTechnique(mat4 ProjectionMatrix, mat4 viewMatrix, mat4 ModelMatrix) override;
	void StopTechnique() override;
	void SetScaleUV(float uv) { ScaleUV = uv; };

private:

	Shader Shader;
	float ScaleUV;
	Light* GlobalLight;
};

