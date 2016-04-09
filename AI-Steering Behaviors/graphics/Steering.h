#pragma once
#include "Agent.h"
#define ANGLE_CHANGE 1.0f
#define SLOWIN_RADIUS 4.0f
#define MAX_SPEED 15.0f
#define DISTANCE_TO_LIDER 2 .5f
#define MAX_DISTANCE_AWAY_FROM_LEADER 7.0f

class Steering
{
public:
	Steering();
	~Steering();
	
	
	static vec3 Cohesion(vector<Entity*> neighbors, Agent* entity);
	static vec3 Separation(vector<Entity*> neighbors, Agent* entity);
	static vec3 Aligment(vector<Entity*> neighbors, Agent* entity);
	static vec3 Wander(Agent* entity);
	static void CheckBoundaries(Agent* entity);
	static vec3 FollowLeader(vector<Entity*> neighbors, Agent* entity);
	
	static void Seek(Agent* entity);
	static void Arrival(vector<Entity*> neighbors, Agent* entity);

private:
	
	
	

};

