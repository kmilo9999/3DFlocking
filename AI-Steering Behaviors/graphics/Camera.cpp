#include "Camera.h"
#include <gtc/matrix_transform.hpp>

Camera::Camera(vec3 position, vec3 up, float yaw, float pitch) :
Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch),Front(glm::vec3(1.0f, 0.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY)
{
	FirstMouse = true;
	UpdateCameraVectors();
}


Camera::~Camera()
{
}


mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::UpdateCameraVectors()
{
	
	vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	Front = glm::normalize(front);
	
	Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(this->Right, this->Front));
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		this->Position += this->Front * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
}

void Camera::ProcessMouseMovement(float xpos, float ypos,  bool moveCamera, bool constrainPitch)
{



	if (FirstMouse)
	{
		LastX = xpos;
		LastY = ypos;
		FirstMouse = false;
	}

	float xoffset = xpos - LastX;
	float yoffset = LastY - ypos;

	LastX = xpos;
	LastY = ypos;

	if (moveCamera){
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		this->Yaw += xoffset;
		this->Pitch += yoffset;


		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}


		UpdateCameraVectors();
	}

	
}

vec3& Camera::GetPosition()
{
	return Position;
}