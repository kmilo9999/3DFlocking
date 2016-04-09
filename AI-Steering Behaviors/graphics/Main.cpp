#include "OpenglGL.h"
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include "Framework.h"
#include "InputHandler.h"
#include <iostream>
using namespace std;



/*
call back method to receive the keyboard events
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		Framework::GetInstance()->running = false;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else{
		InputHandler::GetInstance()->HandleKeyPressedInput(key, action);
	}


}


/*
Main funtion
*/
int main(void)
{


	GLFWwindow* window;
	if (!glfwInit())
		return EXIT_FAILURE;

	window = glfwCreateWindow((int)Framework::GetInstance()->GetRendered()->GetWindowSize().x, (int)Framework::GetInstance()->GetRendered()->GetWindowSize().y, "CDIAZ", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}


	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	


	Framework::GetInstance()->GetRendered()->SetWindow(window);

	Framework::GetInstance()->Init();
	Framework::GetInstance()->Run();




	return 0;
}