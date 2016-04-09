#pragma once
#include "Common.hpp"

struct mytypes{
	vec2  vector2;
	vec3  vector3;
	vec4  vector4;
	mat4  matrix4;
	int  integer;
	float floating;
	bool boolean;

};

class ShadingTechnique
{


public:
	
	
	virtual void LoadShaders()  = 0;
	virtual void StartTechnique() = 0;
	virtual void LoadVariables(map<string, mytypes> variables) = 0;
	virtual void PrepareShadingTechnique(mat4 ProjectionMatrix, mat4 viewMatrix, mat4 ModelMatrix) = 0;
	virtual void StopTechnique() = 0;
};

