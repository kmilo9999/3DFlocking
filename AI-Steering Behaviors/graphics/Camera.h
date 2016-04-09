#pragma once
#include "Common.hpp"


enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 3.0f;
const float SENSITIVTY = 0.25f;


class Camera
{
public:
	Camera(vec3 position = vec3(0.0f, 0.0f, 20.0f), vec3 up = vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	~Camera();
	mat4 GetViewMatrix();
	
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool moveCamera, bool constrainPitch = true);
	

	vec3& GetPosition();

private:
	vec3 Position;
	vec3 Up;
	vec3 Right;
	vec3 Front;
	vec3 WorldUp;

	float Pitch;
	float Yaw;
	
	float MovementSpeed;
	float MouseSensitivity;
	
    
	float  LastX;
	float  LastY;

	void UpdateCameraVectors();
	bool FirstMouse;
	
};

