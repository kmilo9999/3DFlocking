#pragma once
#include "ShadingTechnique.h"
#include "Shader.h"

class Simpletechnique : public ShadingTechnique
{
public:
	Simpletechnique();
	~Simpletechnique();

	void LoadShaders() override;
	void StartTechnique() override;
	void LoadVariables(map<string, mytypes> variables) override;
	void PrepareShadingTechnique(mat4 ProjectionMatrix, mat4 viewMatrix, mat4 ModelMatrix) override;
    void StopTechnique() override;

private :
	Shader shader;
};

