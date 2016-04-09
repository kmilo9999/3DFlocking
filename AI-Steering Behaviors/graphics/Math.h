#pragma once
#include "Common.hpp"
class Math
{
public:
	Math();
	~Math();
	
	static mat4 CreateTransformationMatrix(vec3 position, quat orientation, vec3 scale);
	static quat RotationBetweenVectors(vec3& start, vec3& dest);
	static quat GetRotation(vec3& orig, vec3& dest, vec3& up);
	static vec3 EulerAngles(quat q);
	static float Dot(vec3& v, vec3& w);
	static vec3 Cross(vec3& v, vec3& w);
	static quat Slerp(quat& q1, quat& q2, float t);
	static float Distance(vec3& v, vec3& w);
};

