#pragma once
#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include <time.h>
#include "Steering.h"

class MasterRenderer
{
public:
	MasterRenderer(vec2 windwsSize);
	~MasterRenderer();
	void Init();

	void SetWindow(GLFWwindow*);
	GLFWwindow* GetWindow();

	void SetWindowSize(vec2 v);
	vec2 GetWindowSize();

	void RenderEntities(map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities);
	void RenderTerrains(vector<Terrain*> terrains);

	void Update(map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities, float dt);

	void HandleWindowResize();


	void SetGlobalLight(Light* light);
	Light* GetGlobalLight();

	Camera* GetCamera();

	void UpdateEntites(map<TexturedModel*, vector<Entity*>, TexturedModelCompare> entities, float dt);
	void UpdateCamera(float dt);


	void SetFOV(float fov);

	void PrepareRenderer();
	void FinishRenderer();

	void SetAlignmentWeight(float aligmentW){ AlignmentWeight = aligmentW; };
	void SetCohesionWeight(float cohesionW){ CohesionWeight = cohesionW; };
	void SetSeparationWeight(float sperationW){ SeparationWeight = sperationW; };
	void SetWander(bool wander){ Wander = wander; };
	void SetFollowLeader(bool followL){ FollowL = followL; };
	void SetMoveTarget(bool movetarget){ Movetarget = movetarget; };
	void SetSeek(bool seek){ Seek = seek; };

private: 

	
	void CreateProjectionMatrix();
	void InitOpengl();
	void ProcessMouseScroll(float yoffset);
	EntityRenderer* EntityRender;
	TerrainRenderer* TerrainRender;

	Camera* camera;
	vec2 WindowSize;
	time_t start;
	bool startFlock;
	float AlignmentWeight;
	float CohesionWeight;
	float SeparationWeight;
	bool Wander;
	bool FollowL;
	bool Movetarget;
	bool Seek;

	mat4 ProjectionMatrix;
	map<TexturedModel*, vector<Entity*>> entities;
	GLFWwindow* window;
	Light* GlobalLight;
	
	

	float FOV;


};

