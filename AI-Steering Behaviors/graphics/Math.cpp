#include "Math.h"
#include <gtx/transform.hpp>
#include <gtx/quaternion.hpp>
#include <gtx/norm.hpp>
Math::Math()
{
}


Math::~Math()
{
}



mat4 Math::CreateTransformationMatrix(vec3	translation, quat orientation, vec3 scaleSize)
{
	mat4 translate,rotate,scale;
	translate = glm::translate(mat4(), translation);
	rotate = glm::toMat4(orientation);
	scale = glm::scale(mat4(), scaleSize);

	return translate * rotate * scale;
}

quat Math::RotationBetweenVectors(vec3& start, vec3& dest)
{
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
		if (length2(rotationAxis) < 0.01) 
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return angleAxis(180.0f, rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
		);
}

vec3 Math::Cross(vec3& v, vec3& w)
{
	vec3 result = glm::cross(v, w);
	return result;
}

float Math::Dot(vec3& v, vec3& w)
{
	float result = glm::dot(v,w);
	return result;
}

vec3 Math::EulerAngles(quat q)
{
	return glm::eulerAngles(q);
}

quat Math::GetRotation(vec3& orig, vec3& dest, vec3& up)
{
	
	quat q;
	
	vec3 v0 = orig;
	vec3 v1 = dest;
	v0 = normalize(v0);
	v1 = normalize(v1);

	float d = Math::Dot(v0, v1);
	
	if (d >= 1.0f)
	{
		return q;
	}
	if (d < (1e-6f - 1.0f))
	{
		if (up != vec3(0, 0, 0))
		{
			
			q = angleAxis(180.0f, up);
		}
		else
		{
			// Generate an axis
			vec3 axis = cross(vec3(1, 0, 0), v0);
			if (length(axis) == 0)
			{
				axis = cross(vec3(0, 1, 0), v0);
			}
			axis = normalize(axis);
			q = angleAxis(180.0f, axis);
		}
	}
	else
	{
		float s = sqrt((1 + d) * 2);
		float invs = 1 / s;

		vec3 c = cross(v0, v1);

		q.x = c.x * invs;
		q.y = c.y * invs;
		q.z = c.z * invs;
		q.w = s * 0.5f;

		q = normalize(q);
	}
	return q;
}

quat Math::Slerp(quat& q1, quat& q2, float t)
{
	quat	 out = q2;

	float alpha, beta;
	float cosom = q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w;
	float slerp_epsilon = 0.00001f;

	bool flip;

	if (flip = (cosom < 0))
	{
		cosom = -cosom;
	}

	if ((1.0 - cosom) > slerp_epsilon)
	{
		float omega = acos(cosom);
		float sinom = sin(omega);
		alpha = (float)(sin((1.0 - t) * omega) / sinom);
		beta = (float)(sin(t * omega) / sinom);
	}
	else
	{
		alpha = (float)(1.0 - t);
		beta = (float)t;
	}

	if (flip)
	{
		beta = -beta;
	}

	out.x = (float)(alpha*q1.x + beta*q2.x);
	out.y = (float)(alpha*q1.y + beta*q2.y);
	out.z = (float)(alpha*q1.z + beta*q2.z);
	out.w = (float)(alpha*q1.w + beta*q2.w);

	return out;
}

float Math::Distance(vec3& v, vec3& w)
{
	return length(w-v);
}