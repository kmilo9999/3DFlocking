#include "Agent.h"


Agent::Agent() :separationDistance(1.5f), wanderAngleTheta(0), wanderAnglePhi(0), wanderCircle(2.5f)
{
}


Agent::~Agent() 
{
}

void Agent::Update(float dt)
{
	Velocity = normalize(Velocity);
	Position += Velocity * speed * dt;
}