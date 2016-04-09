#include "EntityRenderer.h"
#include "gtc/matrix_transform.hpp"

EntityRenderer::EntityRenderer()
{

}


EntityRenderer::~EntityRenderer()
{
	if (sun)
	{
		delete sun;
	}
}




void EntityRenderer::Render(map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities, mat4 ProjectionMatrix, mat4 ViewMatrix)
{
	map<TexturedModel*, vector<Entity*>>::iterator it = entities.begin();
	for (it = entities.begin(); it != entities.end(); ++it)
	{
		TexturedModel* model = it->first;
		
		PrepareTextredModel(model);
		vector<Entity*> batch = it->second;
		for (Entity* entity : batch)
		{
			PrepareInstance(it->first, entity, ProjectionMatrix, ViewMatrix);
		}
		UnbindTexturedModel(it->first);
	}
}

void EntityRenderer::PrepareTextredModel(TexturedModel* model)
{
	model->GetShader()->StartTechnique();
	model->Bind();
}

void EntityRenderer::UnbindTexturedModel(TexturedModel* model)
{
	model->UnBind();
	model->GetShader()->StopTechnique();

	
}

void EntityRenderer::PrepareInstance(TexturedModel* model, Entity* entity, mat4 ProjectionMatrix, mat4 ViewMatrix)
{
	mat4 modelMatrix = Math::CreateTransformationMatrix(entity->GetPosition(), entity->GetOrientation(), entity->GetScale());
	model->GetShader()->PrepareShadingTechnique(ProjectionMatrix, ViewMatrix, modelMatrix);
	model->Draw();
}


void EntityRenderer::RenderLight(mat4 ProjectionMatrix, mat4 ViewMatrix)
{
	PrepareTextredModel(sun->GetModel());
	mat4 modelMatrix = Math::CreateTransformationMatrix(sun->GetPosition(), quat(), vec3(1.0f, 1.0f, 1.0f));
	sun->GetModel()->GetShader()->PrepareShadingTechnique(ProjectionMatrix, ViewMatrix, modelMatrix);
	sun->GetModel()->Draw();
	UnbindTexturedModel(sun->GetModel());
	
	
}




Light* EntityRenderer::GetGlobalLight()
{
	return sun;
}

void EntityRenderer::SetGlobalLight(Light* nlight)
{
	sun = nlight;
}