#include "InputHandler.h"
#include <iostream>

InputHandler* InputHandler::instance(0);

InputHandler::InputHandler()
{
	for (int i = 0; i < 1024; ++i)
	{
		keys[i] = false;
	}

	for (int i = 0; i < 2; ++i)
	{
		mouseButtons[i] = false;
	}
}


InputHandler::~InputHandler()
{
}

InputHandler* InputHandler::GetInstance()
{
	if (!instance)
	{
		instance = new InputHandler();
	}
	return instance;
}

void InputHandler::HandleKeyPressedInput(int key, int action)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void InputHandler::HandleMouseButtonInput(int button, int action)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) 
	{
		if (action == GLFW_PRESS)
		{
			mouseButtons[0] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			mouseButtons[0] = false;
		}
		
	}

	
	if (button == GLFW_MOUSE_BUTTON_RIGHT )
	{
		if (action == GLFW_PRESS)
		{
			mouseButtons[1] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			mouseButtons[1] = false;
		}
		
	}
	
}

void InputHandler::HandleMouseMovement(double xpos, double ypos)
{
	MousePos.x = (float)xpos;
	MousePos.y = (float)ypos;
}

void InputHandler::HandleMouseScroll(double xpos, double ypos)
{
	MouseScroll.x = (float)xpos;
	MouseScroll.y = (float)ypos;

}

bool InputHandler::GetKeyState(int key)
{
	return keys[key];
}

bool InputHandler::GetMouseButtonState(int button)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		return mouseButtons[0];
	if (button == GLFW_MOUSE_BUTTON_RIGHT)
		return mouseButtons[1];
}

vec2& InputHandler::GetPosCursorState()
{
	return MousePos;
}

vec2& InputHandler::GetScrollOffsetState()
{
	return MouseScroll;
}

void InputHandler::SetScrollOffsetState(vec2& offset)
{
	MouseScroll = offset;
}