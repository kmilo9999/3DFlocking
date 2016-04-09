#include "Steering.h"
#include "Math.h"
#include <gtx/norm.hpp>
#include <ctime>
#include <iostream>

Steering::Steering() 
{
	srand(static_cast <unsigned> (time(0)));
}


Steering::~Steering()
{
}


void Steering::Seek(Agent* agent)
{
	vec3 fromPiramidToTaget = agent->goal->GetPosition() - agent->GetPosition();
	fromPiramidToTaget = normalize(fromPiramidToTaget);
	vec3 zAxis(0, 0, 1);
	vec3 upVector(0, 1, 0);
	quat q = Math::GetRotation(zAxis, fromPiramidToTaget, upVector);

	quat q1 = agent->GetOrientation();
	agent->SetOrientation(Math::Slerp(q1, q, 0.01f));
	
}

vec3 Steering::Cohesion(vector<Entity*> neighbors, Agent* entity)
{
	vec3 centerOfMass(0.0f, 0.0f, 0.0f);
	vec3 forceVector(0.0f, 0.0f, 0.0f);
	int numNeighbors = 0;
	
	for each (Entity* neighbor in neighbors)
	{
		if (neighbor != entity)
		{
			centerOfMass += neighbor->GetPosition();
			numNeighbors++;
		}
	}

	if (numNeighbors > 0)
	{
		centerOfMass /= numNeighbors;
		forceVector = centerOfMass - entity->GetPosition();
		forceVector = normalize(forceVector);
	}
	
	return forceVector;
	
}

vec3 Steering::Separation(vector<Entity*> neighbors, Agent* entity)
{
	vec3 forceVector(0.0f, 0.0f, 0.0f);
	for each (Entity* neighbor in neighbors)
	{
		if (neighbor != entity)
		{
			float xComponent = (neighbor->GetPosition().x - entity->GetPosition().x);
			float yComponent = (neighbor->GetPosition().y - entity->GetPosition().y);
			float zComponent = (neighbor->GetPosition().z - entity->GetPosition().z);

			float squareDistance = (xComponent  * xComponent ) +
								   (yComponent  * yComponent) +
								   (zComponent  * zComponent);

			if (squareDistance < entity->separationDistance)
			{
				vec3 fordwardVector = entity->GetPosition() - neighbor->GetPosition();
				float scale = length(fordwardVector) / sqrtf(entity->separationDistance);
				forceVector = normalize(fordwardVector) / (1 - scale);
				
			}
		}
	}

	return forceVector;
}

vec3 Steering::Aligment(vector<Entity*> neighbors, Agent* entity)
{
	vec3 forceVector(0.0f, 0.0f, 0.0f);
	int numNeighbors = 0;

	for each (Entity* neighbor in neighbors)
	{
		if (neighbor != entity)
		{
			forceVector += neighbor->GetVelocity();
			numNeighbors++;
		}
	}

	if (numNeighbors > 0)
	{
		forceVector /= numNeighbors;
		forceVector = normalize(forceVector);
	}

	return forceVector;
}

vec3 Steering::Wander(Agent* entity)
{
	CheckBoundaries(entity);
	vec3 forceVector(0.0f, 0.0f, 0.0f);
	
	vec3 circle_center = entity->GetVelocity();
	circle_center = normalize(circle_center);
	circle_center *= entity->wanderCircle;

	vec3 displacement(0,-1,0);
	displacement *= entity->wanderCircle;
	
	float vectorlength = length(displacement);
	displacement.x = sinf(entity->wanderAngleTheta) * cosf(entity->wanderAnglePhi) * vectorlength;
	displacement.y = sinf(entity->wanderAngleTheta) * sinf(entity->wanderAnglePhi) * vectorlength;
	displacement.z = cosf(entity->wanderAngleTheta);
	
	float random1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float random2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	entity->wanderAngleTheta += (random1 * ANGLE_CHANGE) - (ANGLE_CHANGE * 0.5);
	entity->wanderAnglePhi += (random2 * ANGLE_CHANGE) - (ANGLE_CHANGE * 0.5);


	forceVector = circle_center + displacement;

	vec3 zAxis(0, 0, 1);
	vec3 upVector(0, 1, 0);
	quat q = Math::GetRotation(zAxis, forceVector, upVector);

	quat q1 = entity->GetOrientation();
	entity->SetOrientation( Math::Slerp(q1, q, 0.01f));
	
	vec3 Velocity = entity->GetVelocity();
	Velocity += entity->GetOrientation() * vec3(0, 0, 1);
	entity->SetVelocity(Velocity);

	return forceVector;
}

void Steering::CheckBoundaries(Agent* entity)
{
	
	if ((entity->GetPosition().x < -20 || entity->GetPosition().x >20) || 
		(entity->GetPosition().z < -20 || entity->GetPosition().z >20) ||
		(entity->GetPosition().y < 3 || entity->GetPosition().y >9))
	{
		Seek(entity);
	}
}

vec3 Steering::FollowLeader(vector<Entity*> neighbors, Agent* entity)
{
	vec3 forceVector(0, 0, 0);

	if (entity->GetIsLeader())
	{
		Wander(entity);
		vec3 velocity = entity->GetVelocity();
		velocity += entity->GetOrientation() * vec3(0, 0, 1);
		entity->SetVelocity(velocity);
	}
	else
	{
		vec3 followVector;
		vec3 separationForce;
		vec3 cohesionForce;
		vec3 aligmentForce;
		vec3 centerOfMass(0.0f, 0.0f, 0.0f);
		int numNeighbors = 0;

		for each (Entity* neighbor in neighbors)
		{
			Agent* neighbor_agent = dynamic_cast<Agent*>(neighbor);
			if (neighbor_agent->GetIsLeader())
			{
				vec3 leaderVelocity = neighbor_agent->GetVelocity();
				leaderVelocity *= -1;
				leaderVelocity *= 0.5f;
				vec3 behind = neighbor_agent->GetPosition() + leaderVelocity;


				vec3 fromCurrentToTaget = behind - entity->GetPosition();
				fromCurrentToTaget = normalize(fromCurrentToTaget);
				
				float ditance = length(fromCurrentToTaget);
				
				if (ditance < SLOWIN_RADIUS)
				{
					followVector = normalize(fromCurrentToTaget) * MAX_SPEED * (ditance / SLOWIN_RADIUS);
				}
				else
				{
					followVector = normalize(fromCurrentToTaget) * MAX_SPEED;
				}


				// Oriento to leader direction
				vec3 zAxis(0, 0, 1);
				vec3 upVector(0, 1, 0);
				quat q = Math::GetRotation(zAxis, fromCurrentToTaget, upVector);

				quat q1 = entity->GetOrientation();
				entity->SetOrientation(Math::Slerp(q1, q, 0.01f));

			}
			else
			{
				//separationForce = Separation(neighbors, entity);
				// separation
				if (neighbor_agent != entity)
				{
					float xComponent = (neighbor_agent->GetPosition().x - entity->GetPosition().x);
					float yComponent = (neighbor_agent->GetPosition().y - entity->GetPosition().y);
					float zComponent = (neighbor_agent->GetPosition().z - entity->GetPosition().z);

					float squareDistance = (xComponent  * xComponent) +
						(yComponent  * yComponent) +
						(zComponent  * zComponent);

					if (squareDistance < entity->separationDistance)
					{
						vec3 fordwardVector = entity->GetPosition() - neighbor_agent->GetPosition();
						float scale = length(fordwardVector) / sqrtf(entity->separationDistance);
						separationForce = normalize(fordwardVector) / (1 - scale);

					}
				}

				// cohesionForce
				if (neighbor_agent != entity)
				{
						centerOfMass += neighbor_agent->GetPosition();
						aligmentForce += neighbor->GetVelocity();
						numNeighbors++;
				}
			}
		}

		
		
		vec3 Velocity = entity->GetVelocity();
		
		if (numNeighbors > 0)
		{
			centerOfMass /= numNeighbors;
			cohesionForce = centerOfMass - entity->GetPosition();
			cohesionForce = normalize(cohesionForce);

			aligmentForce /= numNeighbors;
			aligmentForce = normalize(aligmentForce);
		}

		
		Velocity = (followVector - Velocity) + separationForce + cohesionForce + aligmentForce;

		Velocity += entity->GetOrientation() * followVector;
		entity->SetVelocity(Velocity);

	}

	return forceVector;
}


void Steering::Arrival(vector<Entity*> neighbors, Agent* entity)
{
	vec3 forceVector(0, 0, 0);
	vec3 fromCurrentToTaget = entity->goal->GetPosition() - entity->GetPosition();
	fromCurrentToTaget = normalize(fromCurrentToTaget);

	float ditance = length(fromCurrentToTaget);

	if (ditance < SLOWIN_RADIUS)
	{
		forceVector = normalize(fromCurrentToTaget) * MAX_SPEED * (ditance / SLOWIN_RADIUS);
	}
	else
	{
		forceVector = normalize(fromCurrentToTaget) * MAX_SPEED;
	}

	vec3 separationForce = Separation(neighbors, entity);

	vec3 zAxis(0, 0, 1);
	vec3 upVector(0, 1, 0);
	quat q = Math::GetRotation(zAxis, fromCurrentToTaget, upVector);

	quat q1 = entity->GetOrientation();
	entity->SetOrientation(Math::Slerp(q1, q, 0.01f));

	vec3 Velocity = entity->GetVelocity();

	Velocity = (forceVector - Velocity) + separationForce;

	Velocity += entity->GetOrientation() * forceVector;
	entity->SetVelocity(Velocity);
}