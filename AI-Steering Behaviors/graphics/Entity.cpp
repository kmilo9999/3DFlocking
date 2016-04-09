#include "Entity.h"


Entity::Entity() :Position(0, 0, 0), Orientation(1, 0, 0, 0), Scale(1, 1, 1), Velocity(0,0,0)
{

}


Entity::Entity( vec3& position, quat orientation, vec3 scale) : Position(position),
Orientation(orientation), Scale(scale),  Velocity(0, 0, 0)
{
	
}

Entity::Entity( vec3& position, float angle, vec3 axis, vec3 scale) :
Position(position), Scale(scale),  Velocity(0, 0, 0)
{
	Orientation = glm::angleAxis(angle, axis);
}

Entity::~Entity()
{
	
}

vec3& Entity::GetPosition() 
{
	return Position;
}
quat& Entity::GetOrientation()  
{
	return Orientation;
}

vec3& Entity::GetScale() 
{
	return Scale;
}

void Entity::SetPosition(vec3& position)
{
	Position = position;
}

void Entity::SetOrientation(quat& orientation)
{
	Orientation = orientation;
}

void Entity::SetOrientation(float angle, vec3& axis)
{
	Orientation = glm::angleAxis(angle, axis);
}

void Entity::SetScale(vec3& scale)
{
	Scale = scale;
}

void Entity::Update(float dt)
{
	if (Velocity != vec3(0,0,0))
	{
		Velocity = normalize(Velocity);
	}
	
	Position += Velocity * speed * dt;
}


vec3& Entity::GetVelocity()
{
	return Velocity;
}

void Entity::SetVelocity(vec3& velocity)
{
	Velocity = velocity;
}
