#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RawModel.h"
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"
#include "Math.h"
#include "InputHandler.h"
#include "Light.h"
#include "ShadingTechnique.h"
#include <map>

class EntityRenderer
{
public:
	EntityRenderer();
	~EntityRenderer();
	

	void RenderLight( mat4 ProjectionMAtrix, mat4 ViewMatrix);
	void Render(map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities, mat4 ProjectionMAtrix, mat4 ViewMatrix);

	

	Light* GetGlobalLight();
	void SetGlobalLight(Light* nlight);

private:

	

	void PrepareTextredModel(TexturedModel* model);
	void UnbindTexturedModel(TexturedModel* model);
	void PrepareInstance(TexturedModel* model, Entity* entity, mat4 ProjectionMatrix, mat4 ViewMatrix);



	


	Light* sun;
};

