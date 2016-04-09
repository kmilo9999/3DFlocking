#pragma once
#include "Entity.h"
class Agent:public Entity
{
public:
	Agent();
	~Agent();
	void Update(float dt) override;
	
	Entity* goal;
	float wanderAngleTheta;
	float wanderAnglePhi;

	float wanderCircle;
	float separationDistance;
	bool GetIsLeader(){ return IsLeader; };
	void SetIsLeader(bool leader){ IsLeader = leader; };
	
private:
	bool IsLeader;
	

};

