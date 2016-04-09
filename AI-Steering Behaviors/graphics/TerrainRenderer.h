#pragma once
#include "Terrain.h"
#include "Light.h"
class TerrainRenderer
{
public:
	TerrainRenderer();
	~TerrainRenderer();
	void Render(vector<Terrain*> terrain, mat4 ProjectionMAtrix, mat4 ViewMatrix);
	Light* GetGlobalLight();
	void SetGlobalLight(Light* nlight);

private:

	void PrepareTextredModel(TexturedModel* model);
	void UnbindTexturedModel(TexturedModel* model);
	void PrepareInstance(TexturedModel* model, Terrain* terrain, mat4 ProjectionMatrix, mat4 ViewMatrix);
	Light* sun;

};

