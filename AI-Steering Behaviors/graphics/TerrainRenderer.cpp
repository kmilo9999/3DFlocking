#include "TerrainRenderer.h"
#include "Math.h"

TerrainRenderer::TerrainRenderer()
{

}


TerrainRenderer::~TerrainRenderer()
{

}

Light* TerrainRenderer::GetGlobalLight()
{
	return sun;
}

void TerrainRenderer::SetGlobalLight(Light* nlight)
{
	sun = nlight;
}

void TerrainRenderer::Render(vector<Terrain*> terrains, mat4 ProjectionMatrix, mat4 ViewMatrix)
{
	vector<Terrain*>::iterator it = terrains.begin();
	for (it = terrains.begin(); it != terrains.end(); ++it)
	{
		Terrain* terrain = *it;
		PrepareTextredModel(terrain->GetModel());
		PrepareInstance(terrain->GetModel(), terrain, ProjectionMatrix, ViewMatrix);
		UnbindTexturedModel(terrain->GetModel());

	}
}

void TerrainRenderer::PrepareTextredModel(TexturedModel* model)
{
	model->GetShader()->StartTechnique();
	model->Bind();
}

void TerrainRenderer::PrepareInstance(TexturedModel* model, Terrain* terrain, mat4 ProjectionMatrix, mat4 ViewMatrix)
{
	vec3 position(terrain->GetX(), 0, terrain->GetZ());
	mat4 modelMatrix = Math::CreateTransformationMatrix(vec3(0,0,0), quat(1,0,0,0),vec3(1,1,1));
	model->GetShader()->PrepareShadingTechnique(ProjectionMatrix, ViewMatrix, modelMatrix);
	model->Draw();
}

void TerrainRenderer::UnbindTexturedModel(TexturedModel* model)
{
	model->UnBind();
	model->GetShader()->StopTechnique();
}