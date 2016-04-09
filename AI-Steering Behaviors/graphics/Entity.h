#pragma once
#include "TexturedModel.h"
#include "Common.hpp"
#include "Shader.h"
//#define speed 2.0f
#define speed 1.5f

class Entity
{
public:
	Entity();
	Entity( vec3& position, quat orientation, vec3 scale);
	Entity( vec3& position, float angle, vec3 axis, vec3 scale);
	~Entity();

	vec3& GetPosition() ;
	quat& GetOrientation()  ;
	vec3& GetScale() ;
	vec3& GetVelocity();
	
	

	void SetPosition(vec3& position);
	void SetOrientation(quat& orientation);
	void SetOrientation(float angle, vec3& axis);
	void SetScale(vec3& scale);
	void SetVelocity(vec3& velocity);

	virtual void Update(float dt);
	

protected:
	
	
	vec3 Position;
	quat Orientation;
	vec3 Scale;
	vec3 Velocity;
};

