#pragma once
#include "Common.hpp"
#include <GLFW/glfw3.h>

class InputHandler
{
public:
	static InputHandler* GetInstance();
	void HandleKeyPressedInput(int key, int action);
	void HandleMouseButtonInput(int button, int action);
	void HandleMouseMovement(double xpos, double ypos);
	void HandleMouseScroll(double xpos, double ypos);
	bool GetKeyState(int key);
	bool GetMouseButtonState(int key);
	vec2& GetPosCursorState();
	vec2& GetScrollOffsetState();
	void SetScrollOffsetState(vec2& offset);

	~InputHandler();

private:
	InputHandler();
	bool keys[1024];
	bool mouseButtons[2];
	vec2 MousePos;
	vec2 MouseScroll;
	
	static InputHandler* instance;
};

